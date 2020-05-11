Compilación cliente Java
  javac cliente.java

Ejecución cliente Java
  java -cp . cliente -s servidor -p puertoServidor
  ej) java -cp . client -s localhost -p 2500

Compilación Servidor y servicioRPC
  make
Ejecución Servidor y servicioRPC
  ./server -p puertoServidor -r hostRPC
  ./rpcServer
  ej) ./server -p 2500 -r localhost
      ./rpcServer

  En terminales distintas

Generación Servicio Web
  javac upper/UpperService.java
  javac upper/UpperPublisher.java

  wsgen -cp . -wsdl upper.UpperService
  wsimport -p client -keep UpperService.wsdl

Ejecución Servicio Web
  java upper.UpperPublisher

ORDEN DE EJECUCIÓN
  1) ./rpcServer
  2) ./server -p puertoServidor -r hostRPC
  3) java upper.UpperPublisher
  4) java -cp . client -s localhost -p 2500
