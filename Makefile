################################################################################
#  A set of template files to organize projects with a single Makefile
#  Copyright (C) 2013 Zhongming Qu <qzmfrank@umich.edu>
#  
#  This program is free software: you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation, either version 3 of the License, or
#  (at your option) any later version.
#  
#  This program is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#  
#  You should have received a copy of the GNU General Public License
#  along with this program.  If not, see <http://www.gnu.org/licenses/>.  
################################################################################
MAKE_DIR:=MAKE/
-include ${MAKE_DIR}Makefile.proj# The top level project file
-include ${MAKE_DIR}Makefile.vars# The top level variables' file
-include ${MKVFILES}# The makevars.mk's in the subdirectories
-include ${DEPFILES}# Dependency files (*.d files)
-include ${MAKE_DIR}Makefile.targets# The top level PHONY targets' file, except for check
###############################################################################
#	Welcome! The comments in this file will guide you through the process.
#  Please read all the comments very carefully before changing anything.
#  	The idea of this set of template files (Makefile, Makefile.vars, and
#  all the makevars.mk) is to help organize any C/C++ project with a single
#  Makefile. The advantage is speed. The usual drawback are bad maintainence
#  and modularity. This set of files attempt to cut down all the repetitive
#  work that are usually done manually, i.e., listing target-prerequisites
#  while preserving almost the same degree of modularity as the recursive
#  make approach.
#  TODO:
#  	Write up python or perl scripts that can scan the src/ directory
#  and generate a single Makefile at the top level. The authors has not done
#  this mainly for two reasons: a) He does not know python or perl any well;
#  b) There is still room for improvement in this set of template files.
#  The author wishes to wait until it
#  	a) is tested on several different Linux clusters, and
#  	b) fully supports dynamic _and_ static libraries and executables, and
#  	c) capable of handling primary configure such as DEBUG and RELEASE, and
#  	d) if possible, supports Fortran files well (to appeal to a larger
#  	audience).
#  
#  	The make utility is a good thing. But GNU make lacks in its fundamental
#  design the respect and support for the hierachy of source file directories
#  that are very compilcated for any project with more than a hundred files.
#  Version control and project build are the two most important deeds in 
#  programming, though often their importance is not fully acknowledged. Version
#  control software had Git replaced SVN and CVS. Maybe the call for a better
#  build tool would in time render the long-lived GNU make obselete. People
#  have to come up with ways to do it right. CMake is very good at portability.
#  However, portability also makes CMake less friendly to many of its quasi-
#  expert users, who, just like the author, would like to be able to modify
#  some of the CMake-generated Makefiles.
#  	What it does is to create a building tool based on GNU make and some
#  script files that can do the build in only one pass, and best of all, without
#  introducing any new syntax. Such a simple goal makes the implementation
#  simple and stable. But it only works on Unix-like platforms that has bash
#  shell and python/perl installed. The author would say that this is a, though
#  not big, limitation.
#  	Unlike many other attempts of creating an universally fit building tool
#  for everyone at everywhere at anytime, e.g., CMake, this set of templates,
#  with possible future scripts, aims to do only thing and do it well. The
#  ultimate goal is to realize building with only one pass.
#  	If you have any suggestions and/or ideas, please let me know.
###############################################################################
