#include "client.h"

typedef struct{
	char tipo_de_animal[20];
	char nombre[20];
	int edad;
	char apodo[20];
} t_mascota ;

int main(void)
{
	/*---------------------------------------------------PARTE 2-------------------------------------------------------------*/

	/* ---------------- LOGGING ---------------- */

	/*t_mascota perrito;

	strcpy(perrito.tipo_de_animal,"Perro");
	strcpy(perrito.nombre,"Camilo");
	strcpy(perrito.apodo,"Milone");
	perrito.edad = 20;

	printf("El tipo de mascota es: %s\n", perrito.tipo_de_animal);
	printf("El nombre es: %s\n", perrito.nombre);
	printf("El apodo es: %s\n", perrito.apodo);
	printf("La edad del %s es: %d\n", perrito.apodo, perrito.edad);
	*/
	t_log* logger = iniciar_logger();

	if(logger==NULL){
		perror("ERROR: No se pudo crear el logger");
		exit(-1);
	}

	// Usando el logger creado previamente
	// Escribi: "Hola! Soy un log"

	log_info(logger,"Hola soy un Log");


	char* palabra = " Bienvenidos :)";
	printf("El config se ha creado correctamente: %s\n",palabra);
	log_info(logger,"Hola soy un Log : %s\n",palabra); // IMPORTANTE PARA LOGGUER

	/* ---------------- ARCHIVOS DE CONFIGURACION ---------------- */

	t_config* config = iniciar_config();

	if(config!=NULL){
		printf("El config se ha creado correctamente");
	}
	else{
		log_error(logger, "No se pudo crear el archivo config");
		exit(-2);
	}

	// Usando el config creado previamente, leemos los valores del config y los 
	// dejamos en las variables 'ip', 'puerto' y 'valor'
	char* valor = config_get_string_value(config,"CLAVE");
	char* ip = config_get_string_value(config,"IP");
	char* puerto = config_get_string_value(config,"PUERTO");

	// Loggeamos el valor de config

	log_info(logger,valor);
	log_info(logger,ip);
	log_info(logger,puerto);


	/* ---------------- LEER DE CONSOLA ---------------- */

	leer_consola(logger);

	/*---------------------------------------------------PARTE 3-------------------------------------------------------------*/

	// ADVERTENCIA: Antes de continuar, tenemos que asegurarnos que el servidor esté corriendo para poder conectarnos a él

	// Creamos una conexión hacia el servidor
	int conexion = crear_conexion(ip, puerto);

	// Enviamos al servidor el valor de CLAVE como mensaje
	enviar_mensaje(valor, conexion);
	//log_info(logger, "Se envio el siguiente mensaje al servidor: %s", valor);

	// Armamos y enviamos el paquete
	paquete(conexion);

	terminar_programa(conexion, logger, config);

	/*---------------------------------------------------PARTE 5-------------------------------------------------------------*/
	// Proximamente
}

t_log* iniciar_logger(void)
{
	t_log* nuevo_logger = log_create("tp0.log","tp0",1,LOG_LEVEL_INFO);
	return nuevo_logger;
}

t_config* iniciar_config(void)
{
	t_config* nuevo_config = config_create("../cliente.config");
	return nuevo_config;
}

void leer_consola(t_log* logger)
{
	char* leido = readline("> ");
	// La primera te la dejo de yapa
	// El resto, las vamos leyendo y logueando hasta recibir un string vacío
	while(strcmp(leido,"")){
		log_info(logger, leido);
		free(leido);
		leido = readline("> ");
	}
	// ¡No te olvides de liberar las lineas antes de regresar!
	free(leido);
}

void paquete(int conexion)
{
	// Ahora toca lo divertido!
	char* leido = readline("> ");
	t_paquete* paquete = crear_paquete(); // creo el paquete para utilizar en el envio del conj de msjs de consola

	// Leemos y esta vez agregamos las lineas al paquete
	while(strcmp(leido,"")){
		agregar_a_paquete(paquete, leido, strlen(leido)+1);
		free(leido);
		leido = readline("> ");
	}
	// ¡No te olvides de liberar las líneas y el paquete antes de regresar!
	free(leido);
	enviar_paquete(paquete, conexion);
	eliminar_paquete(paquete);
}

void terminar_programa(int conexion, t_log* logger, t_config* config)
{
	/* Y por ultimo, hay que liberar lo que utilizamos (conexion, log y config) 
	  con las funciones de las commons y del TP mencionadas en el enunciado */
	if(logger!=NULL){
		log_destroy(logger);
	}
	if(config!=NULL){
		config_destroy(config);
	}
	liberar_conexion(conexion);

}
