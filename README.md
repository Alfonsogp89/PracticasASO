# Práctica: Ampliación de Sistemas Operativos 

Este directorio contiene los materiales y ejemplos correspondientes a la **Semana 1** de prácticas de la asignatura Ampliación de Sistemas Operativos.

## Estructura de la práctica

La práctica se divide en dos partes principales:

### 1. Parte de llamadas al sistema (syscalls)

En esta parte se trabajan conceptos fundamentales de programación en C y el uso de llamadas al sistema en entornos UNIX/Linux. Incluye:

- **Procesamiento de ficheros con `open()`, `read()`, `write()` y `close()`**  
  Aprenderás a manejar archivos a bajo nivel utilizando las llamadas al sistema estándar de UNIX para abrir, leer, escribir y cerrar archivos.  
  Consulta la documentación y ejemplos en `Semana3.md`.

- **Procesamiento de ficheros con `mmap()` y `munmap()`**  
  Se muestra cómo mapear archivos en memoria para acceder a su contenido mediante punteros, facilitando operaciones eficientes sobre grandes volúmenes de datos.  
  Más detalles y ejemplos en `Semana3.md`.

- **Uso de Tuberías (`pipe()`)**  
  Se estudia la comunicación entre procesos mediante tuberías, permitiendo el paso de datos entre procesos productores y consumidores.  
  Ejemplos y explicación detallada en `Semana4.md`.

- **Procesamiento de líneas de órdenes y ejecución de comandos (shell)**  
  Se explica el funcionamiento de un intérprete de órdenes (*shell*), el procesamiento de líneas de comandos, la ejecución de aplicaciones externas y la gestión de procesos hijos.  
  Consulta la documentación y ejemplos en `Semana2.md`.

- **Documentación**:  
  - [Semana1.md](doc/Semana1.md): Explicación sobre la instalación y uso de Visual Studio Code, edición, compilación y depuración de código C, manejo de argumentos de línea de comandos, punteros y referencias útiles.
  - [Semana2.md](../Semana2/doc/Semana2.md): Procesamiento de líneas de órdenes y ejecución de comandos.
  - [Semana3.md](../Semana3/doc/Semana3.md): Procesamiento de ficheros con llamadas al sistema y memoria virtual.
  - [Semana4.md](../Semana4/doc/Semana4.md): Uso de tuberías y señales en UNIX.

- **Archivos fuente**:  
  - Ubicados en `src/`, incluyen ejemplos como `hello.c`, `getopt.c`, `depurame.c`, `punteros.c`, `cat_mem_din.c`, `cat_mem_vir.c`, `shell_ytdl_pipe_ffmpeg.c`, `openpdf.c`, `openpdf+.c`, entre otros.

### 2. Parte de programación en xv6

En esta parte se explora el funcionamiento interno de un sistema operativo tipo UNIX a través del sistema educativo xv6. Se estudian y modifican componentes del kernel relacionados con:

- Implementación y manejo de llamadas al sistema (syscalls) en xv6
- Gestión de procesos, memoria y archivos
- Ejercicios prácticos sobre el código fuente de xv6

El código fuente de xv6 se encuentra en el directorio `/xv6/xv6/` relativo a este README.

## Recomendaciones de uso

- Comienza leyendo la documentación en `doc/Semana1.md`.
- Compila y ejecuta los ejemplos de `src/` para afianzar los conceptos de syscalls en C.
- Consulta y explora el código de xv6 para comprender cómo se implementan las llamadas al sistema a bajo nivel.
- Utiliza Visual Studio Code para editar, compilar y depurar el código, siguiendo las recomendaciones de la documentación.

## Referencias

- [Semana1.md](doc/Semana1.md): Documentación principal de la práctica.
- [Semana2.md](../Semana2/doc/Semana2.md): Procesamiento de líneas de órdenes y shell.
- [Semana3.md](../Semana3/doc/Semana3.md): Procesamiento de ficheros.
- [Semana4.md](../Semana4/doc/Semana4.md): Uso de tuberías y señales.
- [xv6-public](https://pdos.csail.mit.edu/6.828/2018/xv6.html): Página oficial de xv6.

---

**Universidad de Murcia — Grado en Ingeniería Informática**
