@echo off
FOR %%i IN (dot\\*.dot) DO dot -Tpdf %%i -o %%i.pdf