p4 edit ..\..\work\shaders\terrain\*.bin
cd terrain
rem "C:\Program Files\Microsoft Xbox 360 SDK\bin\win32\fxc.exe" "terrainRoads.fx" /Gfp /Xsampreg:0-15,16-24 /T fxl_3_0 /Fo ..\..\..\work\shaders\terrain\terrainRoads.bin
rem "C:\Program Files\Microsoft Xbox 360 SDK\bin\win32\fxc.exe" "terrainRoads.fx" /Gfp /Xsampreg:0-15,16-24 /T fxl_3_0 /Fc terrainRoads.asm 

"C:\Program Files\Microsoft Xbox 360 SDK\bin\win32\fxc.exe" "terrainRoads.fx" /Xsampreg:0-15,16-25 /T fxl_3_0 /Fo ..\..\..\work\shaders\terrain\terrainRoads.bin
"C:\Program Files\Microsoft Xbox 360 SDK\bin\win32\fxc.exe" "terrainRoads.fx" /Xsampreg:0-15,16-25 /T fxl_3_0 /Fc terrainRoads.asm 

cd ..
