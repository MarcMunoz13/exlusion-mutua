using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Net;
using System.Net.Sockets;

namespace WindowsFormsApplication1
{
    public partial class Form1 : Form
    {
        Socket server;
        public Form1()
        {
            InitializeComponent();
        }

        private void Form1_Load(object sender, EventArgs e)
        {

           
        }

   
        private void button2_Click(object sender, EventArgs e) // boton enviar
        {
            
       

                if (DimeJugadores.Checked)
                {
                    // Quiere saber si el nombre es bonito
                    string mensaje = "2/" + Fecha.Text;
                    // Enviamos al servidor el nombre tecleado
                    byte[] msg = System.Text.Encoding.ASCII.GetBytes(mensaje);
                    server.Send(msg);

                    //Recibimos la respuesta del servidor                    
                    byte[] msg2 = new byte[80];
                    server.Receive(msg2);
                    mensaje = Encoding.ASCII.GetString(msg2).Split('\0')[0];


                    if (mensaje == "NO")
                        MessageBox.Show("No hay partidas en esa fecha");
                    else
                        MessageBox.Show("Los jugadores que jugaron ese día son:" + mensaje);


                }
             /*
            else
                {
                    //enviamos nombre y altura
                    string mensaje = "3/" + Fecha.Text + "/" + altura.Text;
                    //enviamos al servidor en conmbre del teclado
                    byte[] msg = System.Text.Encoding.ASCII.GetBytes(mensaje);
                    server.Send(msg);

                    //Recibimos la respuesta del servidor
                    byte[] msg2 = new byte[80];
                    server.Receive(msg2);
                    mensaje = Encoding.ASCII.GetString(msg2).Split('\0')[0];
                    MessageBox.Show(mensaje);
                }

            */


            }
        

        private void button1_Click(object sender, EventArgs e)// boton conectar
        {

            //Creamos un IPEndPoint con el ip del servidor y puerto del servidor 
            //al que deseamos conectarnos
            IPAddress direc = IPAddress.Parse("192.168.56.101");
            IPEndPoint ipep = new IPEndPoint(direc, 9060);


            //Creamos el socket 
            server = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
            try
            {
                server.Connect(ipep);//Intentamos conectar el socket
                this.BackColor = Color.Green;
                MessageBox.Show("Conectado");
            }
                      
                
            
            catch (SocketException)
            {
                //Si hay excepcion imprimimos error y salimos del programa con return 
                MessageBox.Show("No he podido conectar con el servidor");
                return;
            } 

          


        }

        private void button3_Click(object sender, EventArgs e)//boton desconectar
        {
            //Mensaje de desconexion
            string mensaje = "0/";

            byte[] msg = System.Text.Encoding.ASCII.GetBytes(mensaje);
            server.Send(msg);


     
            // Nos desconectamos
            this.BackColor = Color.Gray;
            server.Shutdown(SocketShutdown.Both);
            server.Close();
        }

        private void Fecha_TextChanged(object sender, EventArgs e)
        {

        }

        private void nickcname_TextChanged(object sender, EventArgs e)
        {

        }

        private void button4_Click(object sender, EventArgs e)//botón entrar
        {
            // Quiere saber si el nombre es bonito
            string mensaje = "1/" + nickname.Text + "/" + password.Text;
            // Enviamos al servidor el nombre tecleado
            byte[] msg = System.Text.Encoding.ASCII.GetBytes(mensaje);
            server.Send(msg);

            //Recibimos la respuesta del servidor                    
            byte[] msg2 = new byte[80];
            server.Receive(msg2);
            mensaje = Encoding.ASCII.GetString(msg2).Split('\0')[0];


            if (mensaje == "SI")
                MessageBox.Show("Bienvenido");
            else if (mensaje == "NO")
                MessageBox.Show("No estas registrado. Para registrarte rellena los campos y presiona registrar");
        }
       
        private void button6_Click(object sender, EventArgs e)//cuantos servicios hay
        {
            //pedir numero de servicios realizados
            string mensaje = "4/";

            byte[] msg = System.Text.Encoding.ASCII.GetBytes(mensaje);
            server.Send(msg);


            //Recibimos la respuesta del servidor                    
            byte[] msg2 = new byte[80];
            server.Receive(msg2);
            mensaje = Encoding.ASCII.GetString(msg2).Split('\0')[0];
            contLbl.Text = mensaje;
        }

        private void groupBox1_Enter(object sender, EventArgs e)
        {

        }

        private void DimeJugadores_CheckedChanged(object sender, EventArgs e)
        {

        }

      


     
     
    }
}
