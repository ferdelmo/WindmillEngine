for entry in src/shaders/*.vert
do
  aux="${entry/src/resources}"
  file="${aux/\.vert/\.vert\.spv}"
  "C:\VulkanSDK\1.1.121.2\Bin\glslc.exe" "$entry" -o "$file"
done

for entry in src/shaders/*.frag
do
  aux="${entry/src/resources}"
  file="${aux/\.frag/\.frag\.spv}"
  "C:\VulkanSDK\1.1.121.2\Bin\glslc.exe" "$entry" -o "$file"
done
