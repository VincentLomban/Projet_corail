# Definitions de macros

CXX     = g++
CXXFLAGS = -g -Wall -std=c++11
CXXFILES = lifeform.cc message.cc projet.cc shape.cc simulation.cc 
OFILES = lifeform.o message.o projet.o shape.o simulation.o 

# Definition de la premiere regle

prog: $(OFILES)
	$(CXX) $(OFILES) -o projet

# Definitions de cibles particulieres

depend:
	@echo " *** MISE A JOUR DES DEPENDANCES ***"
	@(sed '/^# DO NOT DELETE THIS LINE/q' Makefile && \
	  $(CXX) -MM $(CXXFLAGS) $(CXXFILES) | \
	  egrep -v "/usr/include" \
	 ) >Makefile.new
	@mv Makefile.new Makefile

clean:
	@echo " *** EFFACE MODULES OBJET ET EXECUTABLE ***"
	@/bin/rm -f *.o *.x *.cc~ *.h~ prog

#
# -- Regles de dependances generees automatiquement
#
# DO NOT DELETE THIS LINE
lifeform.o: lifeform.cc lifeform.h message.h shape.h constantes.h
message.o: message.cc message.h
projet.o: projet.cc simulation.h lifeform.h message.h shape.h \
 constantes.h
shape.o: shape.cc shape.h constantes.h
simulation.o: simulation.cc lifeform.h message.h shape.h constantes.h \
 simulation.h
