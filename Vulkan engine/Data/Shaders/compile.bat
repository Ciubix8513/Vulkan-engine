@echo off

for %%f in (*.vert) do  C:/VulkanSDK/1.2.182.0/Bin32/glslc.exe %%f -o compiled/"%%~nf.spv"
for %%f in (*.frag) do  C:/VulkanSDK/1.2.182.0/Bin32/glslc.exe %%f -o compiled/"%%~nf.spv"

pause
