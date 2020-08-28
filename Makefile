CC = gcc
SRCDIR = src
OBJDIR = obj
INCLDIR = incl
PRCSDIR = prcs
SISTEMA := -D LINUX
CLEAN_COMMAND := rm -f
EJECUTABLEMAIN = ejecutable
EJECUTABLELECTURA = pLectura
EJECUTABLECONVERSION = pConversion
EJECUTABLEFILTRO = pFiltro
EJECUTABLEBINARIZACION = pBinarizacion
EJECUTABLECLASIFICACION = pClasificacion
EJECUTABLEESCRITURA = pEscritura



all: clean main lectura conversion filtro binarizacion clasificacion escritura
	$(eval LOCAL=true)
	@echo "-------------------------------------------------------"
	@echo "Ejecutable generado! Nombre: $(EJECUTABLEMAIN) "
	@echo " "
	@echo "Ejemplo de ejecucion: ./ejecutable -c 1 -u 50 -n 40 -m mascara -b"

main: $(OBJDIR)/main.o $(OBJDIR)/lecturaImagenes.o $(OBJDIR)/filtro.o $(OBJDIR)/escrituraImagenes.o \
	  $(OBJDIR)/conversion.o $(OBJDIR)/clasificacion.o $(OBJDIR)/binarizacion.o
	  $(eval LOCAL=true)
	  @echo "Generando ejecutable main ..."
	  ($(CC) $^ -o $(EJECUTABLEMAIN) -ljpeg && echo "[OK]") \
		||  (echo "[ERROR]" && exit 1; )
	  @echo "-------------------------------------------------------"

lectura: $(OBJDIR)/pLectura.o $(OBJDIR)/lecturaImagenes.o $(OBJDIR)/filtro.o $(OBJDIR)/escrituraImagenes.o \
	  $(OBJDIR)/conversion.o $(OBJDIR)/clasificacion.o $(OBJDIR)/binarizacion.o
	  $(eval LOCAL=true)
	  @echo "Generando ejecutable $(EJECUTABLELECTURA) ..."
	  ($(CC) $^ -o $(EJECUTABLELECTURA) -ljpeg && echo "[OK]") \
		||  (echo "[ERROR]" && exit 1; )

conversion: $(OBJDIR)/pConversion.o $(OBJDIR)/lecturaImagenes.o $(OBJDIR)/filtro.o $(OBJDIR)/escrituraImagenes.o \
	  $(OBJDIR)/conversion.o $(OBJDIR)/clasificacion.o $(OBJDIR)/binarizacion.o
	  $(eval LOCAL=true)
	  @echo "Generando ejecutable $(EJECUTABLECONVERSION) ..."
	  ($(CC) $^ -o $(EJECUTABLECONVERSION) -ljpeg && echo "[OK]") \
		||  (echo "[ERROR]" && exit 1; )

filtro: $(OBJDIR)/pFiltro.o $(OBJDIR)/lecturaImagenes.o $(OBJDIR)/filtro.o $(OBJDIR)/escrituraImagenes.o \
	  $(OBJDIR)/conversion.o $(OBJDIR)/clasificacion.o $(OBJDIR)/binarizacion.o
	  $(eval LOCAL=true)
	  @echo "Generando ejecutable $(EJECUTABLEFILTRO) ..."
	  ($(CC) $^ -o $(EJECUTABLEFILTRO) -ljpeg && echo "[OK]") \
		||  (echo "[ERROR]" && exit 1; )

binarizacion: $(OBJDIR)/pBinarizacion.o $(OBJDIR)/lecturaImagenes.o $(OBJDIR)/filtro.o $(OBJDIR)/escrituraImagenes.o \
	  $(OBJDIR)/conversion.o $(OBJDIR)/clasificacion.o $(OBJDIR)/binarizacion.o
	  $(eval LOCAL=true)
	  @echo "Generando ejecutable $(EJECUTABLEBINARIZACION) ..."
	  ($(CC) $^ -o $(EJECUTABLEBINARIZACION) -ljpeg && echo "[OK]") \
		||  (echo "[ERROR]" && exit 1; )

clasificacion: $(OBJDIR)/pClasificacion.o $(OBJDIR)/lecturaImagenes.o $(OBJDIR)/filtro.o $(OBJDIR)/escrituraImagenes.o \
	  $(OBJDIR)/conversion.o $(OBJDIR)/clasificacion.o $(OBJDIR)/binarizacion.o
	  $(eval LOCAL=true)
	  @echo "Generando ejecutable $(EJECUTABLECLASIFICACION) ..."
	  ($(CC) $^ -o $(EJECUTABLECLASIFICACION) -ljpeg && echo "[OK]") \
		||  (echo "[ERROR]" && exit 1; )

escritura: $(OBJDIR)/pEscritura.o $(OBJDIR)/lecturaImagenes.o $(OBJDIR)/filtro.o $(OBJDIR)/escrituraImagenes.o \
	  $(OBJDIR)/conversion.o $(OBJDIR)/clasificacion.o $(OBJDIR)/binarizacion.o
	  $(eval LOCAL=true)
	  @echo "Generando ejecutable $(EJECUTABLEESCRITURA) ..."
	  ($(CC) $^ -o $(EJECUTABLEESCRITURA) -ljpeg && echo "[OK]") \
		||  (echo "[ERROR]" && exit 1; )

$(OBJDIR)/main.o: $(SRCDIR)/main.c
				@echo "Generando archivos object de $@ ...."
				$(CC) -c $< -o $@ 

$(OBJDIR)/lecturaImagenes.o: $(SRCDIR)/lecturaImagenes.c
				@echo "Generando archivos object de $@ ...."
				$(CC) -c $< -o $@ 

$(OBJDIR)/filtro.o: $(SRCDIR)/filtro.c
				@echo "Generando archivos object de $@ ...."
				$(CC) -c $< -o $@ 

$(OBJDIR)/escrituraImagenes.o: $(SRCDIR)/escrituraImagenes.c
				@echo "Generando archivos object de $@ ...."
				$(CC) -c $< -o $@ 

$(OBJDIR)/conversion.o: $(SRCDIR)/conversion.c
				@echo "Generando archivos object de $@ ...."
				$(CC) -c $< -o $@ 

$(OBJDIR)/clasificacion.o: $(SRCDIR)/clasificacion.c
				@echo "Generando archivos object de $@ ...."
				$(CC) -c $< -o $@ 

$(OBJDIR)/binarizacion.o: $(SRCDIR)/binarizacion.c
				@echo "Generando archivos object de $@ ...."
				($(CC) -c $< -o $@ && echo "[OK]") \
				||  (echo "[ERROR]" && exit 1; )
				$(eval LOCAL=true)

$(OBJDIR)/pLectura.o: $(PRCSDIR)/prcsLectura.c
				@echo "Generando archivos necesarios para los procesos ..."
				@echo " "
				@echo "Generando archivos object de $@ ...."
				$(CC) -c $< -o $@ 

$(OBJDIR)/pConversion.o: $(PRCSDIR)/prcsConversion.c
				@echo "Generando archivos object de $@ ...."
				$(CC) -c $< -o $@ 

$(OBJDIR)/pFiltro.o: $(PRCSDIR)/prcsFiltro.c
				@echo "Generando archivos object de $@ ...."
				$(CC) -c $< -o $@ 

$(OBJDIR)/pBinarizacion.o: $(PRCSDIR)/prcsBinarizacion.c
				@echo "Generando archivos object de $@ ...."
				$(CC) -c $< -o $@ 

$(OBJDIR)/pClasificacion.o: $(PRCSDIR)/prcsClasificacion.c
				@echo "Generando archivos object de $@ ...."
				$(CC) -c $< -o $@ 

$(OBJDIR)/pEscritura.o: $(PRCSDIR)/prcsEscritura.c
				@echo "Generando archivos object de $@ ...."
				$(CC) -c $< -o $@ 

clean:
	$(eval LOCAL=true)
	@echo "Eliminando .out antiguos..."
	@echo >> rm.out
	$(eval LOCAL=true)
	(($(CLEAN_COMMAND) *.out || $(CLEAN_COMMAND_v2) *.out || $(CLEAN_COMMAND_v3) *.out ) \
		&& echo "[OK]") \
		||  (echo "[ERROR]" && exit 1; )

	@echo "Eliminando .o antiguos..."
	(cd $(OBJDIR) && echo >> rm.o )


	(cd $(OBJDIR) && ($(CLEAN_COMMAND) *.o || $(CLEAN_COMMAND_v2) *.o ) \
		&& echo "[OK]") \
		||  (echo "[ERROR]" && exit 1; ) \


	@echo "Limpieza de archivos antiguos completa!!"
	@echo "-------------------------------------------------------"

.SILENT: clean make all main lectura conversion filtro binarizacion clasificacion escritura $(OBJDIR)/*.o\
		$(SRCDIR)/%.c $(PRCSDIR)/%.c 
	
