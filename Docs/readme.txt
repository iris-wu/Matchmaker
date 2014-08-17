This is a QT project, to run it please add this project to your Qt development environment.
 - Open Project, select Matchmaker.pro

- Compile and Run


1. Setting up
- After program starts, to load the mesh, click File->Load Mesh File and then select .obj file. A sample .obj file should come with the project and located in the asset directory of the project
- Next load the texture, click File->Load Texture File. This file can be any size .jpg or png. A sample one is given in the asset directory.

2. Set constraints
- First you need to cut out the surface from the mesh that you want to set the constraints on, select Action->Parameterize Mesh. This will take a few seconds to do.
- Now you can set down your constraint points. To pair them up, first place a constraint point on the mest and then the texture. Only in this order you can add constraint points
- Once you are satisfied with your constraint points, click Action->Triangulate Texture to create triangulation for the constraints on the texture

3. Matching
- Just click on Action->Match for the program to perform a match.
- In the bottom right panel, the path are shown that the match created for the constraints.
- If you want to add new constraints, you need to restart the program and start from scratch. Otherwise performing match again will result in errores results.

4. Embedding
- You can also perfrom the embedding function, by clicking on Action->Embed
- This function only works on correct triangulated constraints. Thus only part of the mesh will be embedded.

Known limitations:

The Matching algorithm can't create all possible triangulations since only 4.1 from the Matchmaker.pdf was implemented. Thus only path are created that don't require Steiner vertices.
Since the matching was not completed a final output with the mapped texture onto the mesh is not available either.