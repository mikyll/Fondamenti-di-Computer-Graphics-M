# Setup

## Windows

### Visual Studio
1. Create new project
2. Select "Empty Project" (C++)
3. Give a name to the project and the solution
4. Open the Package Manager Console `Tools > NuGet Package Manager > Package Manager Console`
5. Install packages
    1. nupengl.core
        ```powershell
          install-package nupengl.core
        ```
    2. glm
        ```powershell
          install-package glm
        ```

Alternatively, you can also use the **NuGet package manager** to browse and install packages:
![image](https://github.com/mikyll/Fondamenti-di-Computer-Graphics-M/assets/56556806/d107c66d-9f13-4119-bfd5-90922fba0a9d)


#### (Optional) Configure Debug Current Project
1. Go to `Tools > Options > Environment > Keyboard`
2. Search for the command "ClassViewContextMenus.ClassViewProject.Debug.StartNewInstance"
3. Assign a new shortcut key/s, for example `Shift + F5`
4. Now you can launch a new debug instance on the project of the currently open file by clicking your key/s

NB: that's mostly useful when you have multiple projects in the same solution, so you don't have to change the *startup project* each time.
