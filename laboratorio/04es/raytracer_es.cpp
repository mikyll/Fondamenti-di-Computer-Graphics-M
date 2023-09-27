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
bool
RayTracer::CastRay (Ray & ray, Hit & h, bool use_sphere_patches) const
{
  bool answer = false;
  Hit nearest;
  nearest = Hit();

  // intersect each of the quads
  for (int i = 0; i < mesh->numQuadFaces (); i++)
  {
	Face *f = mesh->getFace (i);
	if (f->intersect (ray, h, args->intersect_backfacing))
	{
		if( h.getT() < nearest.getT() )
		{
			answer = true;
			nearest = h;
		}
	}
  }

  // intersect each of the spheres (either the patches, or the original spheres)
  if (use_sphere_patches)
  {
	for (int i = mesh->numQuadFaces (); i < mesh->numFaces (); i++)
	{
	  Face *f = mesh->getFace (i);
	  if (f->intersect (ray, h, args->intersect_backfacing))
	  {
		if( h.getT() < nearest.getT() )
		{
			answer = true;
			nearest = h;
		}
	  }
	}
  }
  else
  {
	const vector < Sphere * >&spheres = mesh->getSpheres ();
	for (unsigned int i = 0; i < spheres.size (); i++)
	{
	  if (spheres[i]->intersect (ray, h))
	  {
		if( h.getT() < nearest.getT() )
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
Vec3f RayTracer::TraceRay (Ray & ray, Hit & hit, int bounce_count) const
{

  hit = Hit ();
  bool intersect = CastRay (ray, hit, false);

  if( bounce_count == args->num_bounces )
  	RayTree::SetMainSegment (ray, 0, hit.getT () );
  else
	RayTree::AddReflectedSegment(ray, 0, hit.getT() );

  Vec3f answer = args->background_color;

  Material *m = hit.getMaterial ();
  if (intersect == true)
  {
	assert (m != NULL);
	Vec3f normal = hit.getNormal ();
	Vec3f point = ray.pointAtParameter (hit.getT ());

	// ----------------------------------------------
	// ambient light
	answer = args->ambient_light * m->getDiffuseColor ();

	// ----------------------------------------------
	// if the surface is shiny...
	Vec3f reflectiveColor = m->getReflectiveColor ();

	// ==========================================
	// ASSIGNMENT:  ADD REFLECTIVE LOGIC
	// ==========================================
	
	// se (il punto sulla superficie e' riflettente & bounce_count>0)
	// NB: per questo basta che controllo se la Length del vettore è != 0 (ovvero non ha tutte le componenti nulle) e che non sia arrivato all'ultimo bounce (ultimo step della ricorsione)
	if (reflectiveColor.Length() != 0 && bounce_count > 0)
	{
		Vec3f VRay = ray.getDirection();
		
		//     calcolare ReflectionRay  R=2<n,l>n -l
		// NB: ReflectionRay è rv a slide 28
		Vec3f reflectionRay = VRay - (2 * VRay.Dot3(normal) * normal);
		reflectionRay.Normalize();
		Ray* new_ray = new Ray(point, reflectionRay);

		//	   invocare TraceRay(ReflectionRay, hit, bounce_count-1)
		//     aggiungere ad answer il contributo riflesso
		answer += TraceRay(*new_ray, hit, bounce_count - 1) * reflectiveColor;
	}
	
	// ----------------------------------------------
	// add each light
	int num_lights = mesh->getLights ().size ();
	for (int i = 0; i < num_lights; i++)
	{
	  // ==========================================
	  // ASSIGNMENT:  ADD SHADOW LOGIC
	  // ==========================================
		Hit* new_hit;
		bool colpito;
		Ray* n_ray;
		Vec3f n_point, dista, pointOnLinght;


	  Face *f = mesh->getLights ()[i];
	  Vec3f pointOnLight = f->computeCentroid ();
	  Vec3f dirToLight = pointOnLight - point;
	  dirToLight.Normalize ();

	  n_ray = new Ray(point, dirToLight);
	  new_hit = new Hit();
	  // colpisco qualcosa che va a finire in new_hit
	  colpito = CastRay(*n_ray, *new_hit, false);

	  if (colpito)
	  {
		  n_point = n_ray->pointAtParameter(new_hit->getT());

		  // calcola il vettore distanza fra il punto colpito dal raggio e il punto sulla luce
		  dista.Sub(dista, n_point, pointOnLight); // calcolo la distanza fra i due punti: punto colpito dallo shadow ray e il punto sul baricentro della luce (pointOnLight)

		  // Se la norma del vettore è molto vicino a zero (non facciamo mai zero completo) signfica che dal punto colpito vedo la luce
		  if (dista.Length() < 0.01)
		  {
			  // se vedo la luce significa che devo andare a calcolare (con Phong) la luce incidente e il contributo locale di quella luce su quel punto
			  if (normal.Dot3(dirToLight) > 0)
			  {
				  Vec3f lightColor = 0.2 * f->getMaterial()->getEmittedColor() * f->getArea();
				  // NB: in answer ci avevamo già messo il contributo ambiente, ora aggiungiamo anche questo.
				  answer += m->Shade(ray, hit, dirToLight, lightColor, args);
			  }
		  }

	  }

      // creare shadow ray verso il punto luce
	  
	  // controllare il primo oggetto colpito da tale raggio

	  // se e' la sorgente luminosa i-esima allora
	  //	calcolare e aggiungere ad answer il contributo luminoso
	  // altrimenti
	  //    la luce i non contribuisce alla luminosita' di point.

	  if (normal.Dot3 (dirToLight) > 0)
	  {
		Vec3f lightColor = 0.2 * f->getMaterial ()->getEmittedColor () * f->getArea ();
		answer += m->Shade (ray, hit, dirToLight, lightColor, args);
	  }
	}
    
  }

  return answer;
}