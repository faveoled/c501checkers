SUBDIRS = checkers data locale win32 linux-menu

bin_PROGRAMS=c501checkers

INCLUDES = -Icheckers $(CFLAGS_ALL)

c501checkers_LDFLAGS = $(EXTRA_LDFLAGS)
	
c501checkers_LDADD = checkers/libcheckers.la $(LIBS_ALL)

c501checkersSrcBase = App.cpp MainFrame.hpp AppConfig.hpp \
	App.hpp MainFrame.cpp MainPanel.hpp MainPanel.cpp \
	findFile.hpp findFile.cpp config.hpp DialogAbout.hpp \
	DialogAbout.cpp DialogConfig.hpp DialogConfig.cpp \
	strWxStdConv.hpp strWxStdConv.cpp \
	SfmlBoard.cpp SfmlBoard.hpp wxSFMLCanvas.cpp wxSFMLCanvas.hpp


## --------------- Resources windows ---------------
.rc.o:
	"$(WINDRES)" $^ -o $@ $(EXTRA_WINDRES_ARG)

if HAVE_WINDRES
c501checkers_SOURCES=$(c501checkersSrcBase) rc.rc
else
c501checkers_SOURCES=$(c501checkersSrcBase)
endif
## --------------- Resources windows --------------- 

man_MANS = c501checkers.1

if IS_MINGW
install-data-hook:
	## Install Dlls
	"$(srcdir)/installDll.sh" "$(prefix)" "$(host)"
endif

EXTRA_DIST = $(srcdir)/*.cbp $(man_MANS) $(srcdir)/rc.rc $(srcdir)/installDll.sh
