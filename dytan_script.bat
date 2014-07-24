DEL dytan.dll
DEL obj-ia32\dytan.dll
..\nmake tools
REN ..\Nmakefile Nmakefile3
REN ..\Nmakefile2 Nmakefile
..\nmake dytan.dll
REN ..\Nmakefile Nmakefile2
REN ..\Nmakefile3 Nmakefile
COPY obj-ia32\dytan.dll .\