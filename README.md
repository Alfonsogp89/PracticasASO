# Práctica: Ampliación de Sistemas Operativos • Semana 1

Este directorio contiene los materiales y ejemplos correspondientes a la **Semana 1** de prácticas de la asignatura Ampliación de Sistemas Operativos.

## Estructura de la práctica

La práctica se divide en dos partes principales:

### 1. Parte de llamadas al sistema (syscalls)

En esta parte se trabajan conceptos fundamentales de programación en C y el uso de llamadas al sistema en entornos UNIX/Linux. Incluye:

- **Documentación**:  
  - [Semana1.md](doc/Semana1.md): Explicación detallada sobre la instalación y uso de Visual Studio Code, edición, compilación y depuración de código C, manejo de argumentos de línea de comandos, punteros y referencias útiles.
- **Ejercicios y ejemplos en C**:
  - Procesamiento de argumentos con `getopt()`
  - Gestión de memoria dinámica con `malloc()` y `free()`
  - Manejo de punteros
  - Ejemplos de compilación, ejecución y depuración
- **Archivos fuente**:  
  - Ubicados en `src/`, incluyen ejemplos como `hello.c`, `getopt.c`, `depurame.c`, `punteros.c`, entre otros.

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
- [xv6-public](https://pdos.csail.mit.edu/6.828/2018/xv6.html): Página oficial de xv6.

---

**Universidad de Murcia — Grado
