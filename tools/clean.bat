: =============================================================================
: Copyright (c) 2010-2014 Bolero MURAKAMI
: https://github.com/bolero-MURAKAMI/KTL
:
: Distributed under the Boost Software License, Version 1.0. (See accompanying
: file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
: =============================================================================
@echo clean files...
@setlocal

@cd ..\projects
@for /r %%d in (Debug) do @rd /s /q "%%d"
@for /r %%d in (Release) do @rd /s /q "%%d"
@del /s /q *.ncb

@cd ..\lib
@del /s /q *.exp
@del /s /q *.pdb
@del /s /q *.lib
