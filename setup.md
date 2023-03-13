# Setup

## Windows

### Visual Studio
1. Create new project
2. Select "Console App" (C++) project
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

#### (Optional) Configure Debug Current Project
1. Go to `Tools > Options > Environment > Keyboard`
2. Search for the command "ClassViewContextMenus.ClassViewProject.Debug.StartNewInstance"
3. Assign a new shortcut key/s, for example `Shift + F5`
