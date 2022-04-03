
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <mysql.h>
#include <stdlib.h>
#include <pthread.h>
//#include <my_global.h>




//#include <mysql.h>
//#include <string.h>
//#include <stdlib.h>
//#include <stdio.h>
int contador;

//estructura para acceso excluyente
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;



void *AtenderCliente (void *socket)
{
	
	
	//inizializamos conexion con la base de datos
	MYSQL *conn;
	int err;
	// Estructura especial para almacenar resultados de consultas 
	MYSQL_RES *resultado;
	MYSQL_ROW row;
	//Creamos una conexion al servidor MYSQL 
	conn = mysql_init(NULL);
	if (conn==NULL) {
		printf ("Error al crear la conexion: %u %s\n", 
				mysql_errno(conn), mysql_error(conn));
		exit (1);
	}
	//inicializar la conexin
	conn = mysql_real_connect (conn, "localhost","root", "mysql", "CM_BD",0, NULL, 0);
	if (conn==NULL) {
		printf ("Error al inicializar la conexion: %u %s\n", 
				mysql_errno(conn), mysql_error(conn));
		exit (1);
	}
	
	
	
	
	
	
	
	
	
	int sock_conn;
	int *s;
	s= (int *) socket;
	sock_conn= *s;
	
	
	
	char peticion[512];
	char respuesta[512];
	int ret;
	
	
	int terminar =0;
	//entramos en bucle para atender a todas las peticiones de este cliente hasta que se desconecte
	
	
	while (terminar ==0)
	{
		
		// Ahora recibimos su peticion
		ret=read(sock_conn,peticion, sizeof(peticion)); //guarda en petici￳n, vector de caracteres
		printf ("Recibida una peticion\n");
		
		// Tenemos que a?adirle la marca de fin de string 
		// para que no escriba lo que hay despues en el buffer
		peticion[ret]='\0';
		
		//Escribimos la peticion en la consola
		
		printf ("La peticion es: %s\n",peticion);
		char *p = strtok(peticion, "/");//arrancar numero (puntero )
		int codigo =  atoi (p);//convierte en entero y introduce en codigo
		
		
		
		if (codigo ==0)
			terminar=1;
		
		else if (codigo== 4)
		{
			sprintf (respuesta, "%d", contador);
			write (sock_conn,respuesta, strlen(respuesta));
		}
		
		else if (codigo ==1) //log in
		{
			char nick[20];
			p = strtok( NULL, "/");
			strcpy(nick, p);
			char passw[20];
			p = strtok(NULL, "/");
			strcpy(passw, p);
			char consulta[100];
			
			
			
			
			
			sprintf(consulta, "SELECT jugadores.Nombre FROM jugadores WHERE jugadores.Nickname = '%s' AND jugadores.Contrase￱a = '%s'", nick, passw);
			
			err=mysql_query (conn, consulta);
			if (err!=0) {
				printf ("Error al consultar datos de la base %u %s\n",
						mysql_errno(conn), mysql_error(conn));
				exit (1);
			}
			
			
			printf("consulta: %s\n",consulta);
			
			err= mysql_query (conn, consulta);
			resultado = mysql_store_result (conn);
			row = mysql_fetch_row (resultado);
			
			
			if (row == NULL)
				sprintf(respuesta, "NO");
			else
				strcpy(respuesta, "SI");
			
			//write (sock_conn,respuesta, strlen(respuesta));
			
			
			
			printf ("Respuesta: %s\n", respuesta);
			// Enviamos la respuesta
			write (sock_conn,respuesta, strlen(respuesta));
			
		}
		/*
		else if (codigo ==2)
		{
		char fecha[30];
		p= strtok(NULL, "/");
		strcpy(fecha,p);
		
		char consulta[100];
		
		sprintf (consulta, "SELECT jugadores.Nombre FROM jugadores,partidas,datos WHERE partidas.Fecha = '%s' AND partidas.id=datos.idP AND (datos.idJ1 = jugadores.id OR datos.idJ2 = jugadores.id)",fecha);
		
		err=mysql_query (conn, consulta);
		if (err!=0) {
		printf ("Error al consultar datos de la base %u %s\n",
		mysql_errno(conn), mysql_error(conn));
		exit (1);
		}
		
		printf("consulta: %s\n",consulta);
		
		err= mysql_query (conn, consulta);
		resultado = mysql_store_result (conn);
		row = mysql_fetch_row (resultado);
		
		if (row == NULL)
		{
		sprintf (respuesta, "NO");
		write (sock_conn,respuesta, strlen(respuesta));
		
		}
		else
		{
		
		while (row !=NULL) 
		{
		// la columna 0 contiene el nombre del jugador
		sprintf (resultado, "%s\n", row[0]);
		// obtenemos la siguiente fila
		row = mysql_fetch_row (resultado);
		
		
		}
		
		write (sock_conn,resultado, strlen(resultado));
		
		}
		
		
		}
		
		*/
		
		
		
		
		
		
		
		
		if ((codigo==1)||(codigo==2))
		{
			pthread_mutex_lock (&mutex);//no interrumpas ahora
			contador=contador+1;
			pthread_mutex_unlock ( &mutex); //puedes interrumpir
		}
		// Se acabo el servicio para este cliente
		
	}
	close(sock_conn); 
}
	



	
int main(int argc, char **argv)
{	
	
	int sock_conn, sock_listen;
	struct sockaddr_in serv_adr;
	
	// INICIALITZACIONS
	// Obrim el socket
	if ((sock_listen = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		printf("Error creant socket");
	}
	// Fem el bind al port
	
	
	memset(&serv_adr, 0, sizeof(serv_adr));// inicialitza a zero serv_addr
	serv_adr.sin_family = AF_INET;
	
	
	// asocia el socket a cualquiera de las IP de la m?quina. 
	//htonl formatea el numero que recibe al formato necesario
	serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
	// escucharemos en el port 9050
	serv_adr.sin_port = htons(9040);//puertopuertopuertopuertopuertopuertopuertopuertopuertopuertopuertopuertopuertopuertopuertopuertopuertopuertopuertopuertopuertopuertopuertopuertopuertopuertopuertopuertopuertopuerto
	if (bind(sock_listen, (struct sockaddr *) &serv_adr, sizeof(serv_adr)) < 0)
		printf ("Error al bind");
	
	
	//La cola de peticiones pendientes no podr? ser superior a 4
	if (listen(sock_listen, 4) < 0)
		printf("Error en el Listen");
	
	contador =0;
	int i;
	
	int sockets[100];
	pthread_t thread; //creo la estuctura de threads y declaro un vector de threads, en creador de threads incluyo el que estamos usando ahora
	
	// Atenderemos solo 10 peticione
	for(;;){ 
		printf ("Escuchando\n");
		
		sock_conn = accept(sock_listen, NULL, NULL);
		printf ("He recibido conexion\n");
		
		
		
		sockets[i]= sock_conn;
		

		pthread_create (&thread, NULL, AtenderCliente, &sockets[i]);//env￭o el socket del vector que acabo de crear por referencia
		
		i++;
		
		
	}
	//for (i=0; i<5; i++)
		//pthread_join (thread[i], NULL);
	
}



