Compilación cliente Java
  javac cliente.java

Ejecución cliente Java
  java . -cp cliente -s servidor -p puertoServidor
  ej) java . -cp cliente -s localhost -p 2500

Compilación Servidor y servicioRPC
  make
Ejecución Servidor y servicioRPC
  ./server -p puertoServidor -r hostRPC
  ej) ./server -p 2500 -r localhost

Generación Servicio Web
  javac upper/UpperService.java
  javac upper/UpperPublisher.java

  wsgen -cp . -wsdl upper.UpperService
  wsimport -p client -keep UpperService.wsdl

Ejecución Servicio Web
  java upper.UpperPublisher
