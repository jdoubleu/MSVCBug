# MSVC frontend crash reproduction
https://developercommunity.visualstudio.com/t/Frontend-crash-when-compiling-nested-tem/10612923

This repository reproduces a MSVC compiler crash in the frontend:

```
1>main.cpp
1>C:\Projects\MSVCBug\main.cpp(12,1): fatal  error C1001: Internal compiler error.
1>(compiler file 'msc1.cpp', line 1587)
1> To work around this problem, try simplifying or changing the program near the locations listed above.
1>If possible please provide a repro here: https://developercommunity.visualstudio.com
1>Please choose the Technical Support command on the Visual C++
1> Help menu, or open the Technical Support help file for more information
1>INTERNAL COMPILER ERROR in 'C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.39.33519\bin\HostX64\x64\CL.exe'
1>    Please choose the Technical Support command on the Visual C++
1>    Help menu, or open the Technical Support help file for more information
1>Done building project "MSVCBug.vcxproj" -- FAILED.
```

You can also observe this in the GitHub CI: https://github.com/jdoubleu/MSVCBug/actions/runs/8224493505/job/22488396152#step:4:31

GCC and Clang are able to compile the code: https://godbolt.org/z/ajvj5jh68

## Origin
This crash might be caused by the following properties, in combination:
1. the 2nd level nested tempalted IIFE inside the fold expansion.
2. usage of the `this` pointer inside the nested lambdas.

If you change the [`this` assignment](./main.cpp#L20), as commented, it doesn't crash. Likewise, if you remove the outer lambda, it doesn't crash neither.

## Bonus
The code doesn't compiler when you remove the `template` from [`.template operator()<..`](./main.cpp#L21), which might be fine. The error messages aren't really helpful here. GCC hints, that you need to use `template`. Maybe this could be improved too.