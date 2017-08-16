call "\Program Files (x86)\Microsoft Visual Studio 14.0\VC\bin\vcvars32.bat"
pushd juce\extras\Projucer\Builds\VisualStudio2015
msbuild Projucer.sln
"x64/Debug/Projucer.exe" ..\..\..\..\..\Life.jucer
popd
