@echo off
FOR %%i IN (conflict_graph\\*.dot) DO dot -Tpdf %%i -o %%i.pdf
:echo "successfull!"
:pause%