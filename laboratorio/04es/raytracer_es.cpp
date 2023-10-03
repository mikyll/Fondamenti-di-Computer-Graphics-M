#include "raytracer.h"
#include "material.h"
#include "vectors.h"
#include "argparser.h"
#include "raytree.h"
#include "utils.h"
#include "mesh.h"
#include "face.h"
#include "sphere.h"

// casts a single ray through the scene geometry and finds the closest hit
bool RayTracer::CastRay (Ray& ray, Hit& h, bool use_sphere_patches) const
{
	bool answer = false;
	Hit nearest;
	nearest = Hit();

	// Intersect each of the quads
	for (int i = 0; i < mesh->numQuadFaces(); i++)
	{
		Face *f = mesh->getFace (i);
		if (f->intersect (ray, h, args->intersect_backfacing))
		{
			if(h.getT() < nearest.getT())
			{
				answer = true;
				nearest = h;
			}
		}
	}

	// intersect each of the spheres (either the patches, or the original spheres)
	if (use_sphere_patches)
	{
		for (int i = mesh->numQuadFaces(); i < mesh->numFaces(); i++)
		{
			Face *f = mesh->getFace(i);
			if (f->intersect (ray, h, args->intersect_backfacing))
			{
				if( h.getT() < nearest.getT())
				{
					answer = true;
					nearest = h;
				}
			}
		}
	}
	else
	{
		const vector < Sphere * > &spheres = mesh->getSpheres ();
		for (unsigned int i = 0; i < spheres.size (); i++)
		{
			if (spheres[i]->intersect(ray, h))
			{
				if(h.getT() < nearest.getT())
				{
					answer = true;
					nearest = h;
				}
			}
		}
	}

	h = nearest;
	return answer;
}


// questo metodo calcola i contributi locali che incidono sul punto
Vec3f RayTracer::TraceRay(Ray& ray, Hit& hit, int bounce_count) const
{
	hit = Hit();
	bool intersect = CastRay(ray, hit, false);

	if (bounce_count == args->num_bounces)
	{
		RayTree::SetMainSegment(ray, 0, hit.getT());
	}
	else
	{
		RayTree::AddReflectedSegment(ray, 0, hit.getT());
	}

	Vec3f answer = args->background_color;

	Material *m = hit.getMaterial();
	if (intersect == true)
	{
		assert(m != NULL);
		Vec3f normal = hit.getNormal();
		Vec3f point = ray.pointAtParameter (hit.getT());

		// ----------------------------------------------
		// ambient light
		answer = args->ambient_light * m->getDiffuseColor();

		// ----------------------------------------------
		// if the surface is shiny...
		Vec3f reflectiveColor = m->getReflectiveColor();

		// ==========================================
		// ASSIGNMENT:  ADD REFLECTIVE LOGIC
		// ==========================================
	
		// Se (il punto sulla superficie e' riflettente e non siamo arrivati all'ultimo
		// bounce (ultimo step della ricorsione)
		// NB: per questo basta che controllo se la Length del vettore reflective 
		// dell'oggetto colpito è != 0 (ovvero il suo materiale non ha tutte le componenti 
		// del vettore reflective nulle) e che il numero di bounce ancora da effettuare 
		// sia > 0.
		if (reflectiveColor.Length() != 0 && bounce_count > 0)
		{
			Vec3f VRay = ray.getDirection();
		
			// Calcolare reflectionRay: R = 2<n,l>n -l
			// NB: ReflectionRay è rv a slide 28 (pdf PBR - Ray Tracing)
			Vec3f reflectionRay = VRay - (2 * VRay.Dot3(normal) * normal);
			reflectionRay.Normalize();
			Ray* new_ray = new Ray(point, reflectionRay);

			// Invocare TraceRay(ReflectionRay, hit, bounce_count-1)
			// aggiungere ad answer il contributo riflesso
			answer += TraceRay(*new_ray, hit, bounce_count - 1) * reflectiveColor;
		}
	
		// ----------------------------------------------
		// Add each light
		
		// ==========================================
		// ASSIGNMENT:  ADD SHADOW LOGIC
		// ==========================================
		int num_lights = mesh->getLights().size();
		Ray* shadow_ray;
		Hit* new_hit;
		bool colpito;
		Vec3f n_point, dista, pointOnLight;		

		// Hard shadows
		if (!args->softShadow)
		{
			// Dobbiamo considerare tutti i contributi luminosi, dunque 
			// facciamo un ciclo per ciascuna luce in scena
			for (int i = 0; i < num_lights; i++)
			{
				Face* f = mesh->getLights()[i];
				// Trattiamo la luce come puntiforme, anche se è un'area
				pointOnLight = f->computeCentroid();
				Vec3f dirToLight = pointOnLight - point;
				dirToLight.Normalize();

				// Creiamo uno shadow ray verso il punto luce
				shadow_ray = new Ray(point, dirToLight);

				// Casto il raggio e controllo il primo oggetto colpito (se esiste) da questo
				new_hit = new Hit();
				colpito = CastRay(*shadow_ray, *new_hit, false);
				// Controllo se il raggio ha colpito qualcosa (che viene salvato in new_hit)
				if (colpito)
				{
					// E' stato colpito qualcosa, allora voglio sapere se l'oggetto colpito è
					// la sorgente luminosa (i-esima, nel ciclo for), oppure un altro oggetto.
					// Per scoprirlo, controllo

					n_point = shadow_ray->pointAtParameter(new_hit->getT());

					// Calcola il vettore distanza fra il punto colpito dallo shadow ray e il 
					// punto sul baricentro della luce (pointOnLight)
					dista.Sub(dista, n_point, pointOnLight);

					// Se la norma del vettore è molto vicino a zero (non consideriamo mai 
					// zero completo) signfica che dal punto colpito vedo la luce
					if (dista.Length() < 0.01)
					{
						// Dal punto colpito si vede la luce (in linea diretta), dunque la fonte
						// luminosa i-esima contribuisce all'illuminazione del punto considerato

						// se vedo la luce significa che devo andare a calcolare (con Phong) la
						// luce incidente e il contributo locale di quella luce su quel punto
						if (normal.Dot3(dirToLight) > 0)
						{
							// Aggiungiamo il contributo della luce
							Vec3f lightColor = 0.2 * f->getMaterial()->getEmittedColor() * f->getArea();

							// NB: in answer ci avevamo già messo il contributo ambiente, ora
							// aggiungiamo anche questo.
							answer += m->Shade(ray, hit, dirToLight, lightColor, args);
						}
					}
					// else: la luce i-esima non contribuisce alla luminosità di point.
				}
			}
		}
		// Soft shadows
		else
		{
			int num_points = 100;

			for (int i = 0; i < num_lights; i++)
			{
				Face* f = mesh->getLights()[i];

				std::vector<Vec3f> pointsOnLigh;
				std::vector<Vec3f> directionsToLight;

				// Random points on area light
				for (int j = 0; j < num_points; j++)
				{
					pointsOnLigh.push_back(f->RandomPoint());
					Vec3f dirToLight = pointsOnLigh.at(j) - point;
					dirToLight.Normalize();
					directionsToLight.push_back(dirToLight);
				}

				for (int j = 0; j < num_points; j++)
				{
					shadow_ray = new Ray(point, directionsToLight.at(j));
					new_hit = new Hit();
					colpito = CastRay(*shadow_ray, *new_hit, false);

					if (colpito)
					{
						n_point = shadow_ray->pointAtParameter(new_hit->getT());
						dista.Sub(dista, n_point, pointsOnLigh.at(j));

						if (dista.Length() < 0.01 && normal.Dot3(directionsToLight.at(j)) > 0)
						{
							Vec3f lightColor = 0.2 / num_points * f->getMaterial()->getEmittedColor() * f->getArea();
							answer += m->Shade(ray, hit, directionsToLight.at(j), lightColor, args);
						}
					}
				}
			}
		}
	}

	return answer;
}