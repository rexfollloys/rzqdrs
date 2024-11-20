/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/Classes/Class.java to edit this template
 */
import java.rmi.registry.LocateRegistry;
import java.rmi.registry.Registry;
import java.io.IOException;
import java.net.*;  
import java.util.logging.Level;
import java.util.logging.Logger;
/**
 *
 * @author lmercader
 */
public class Client {
    public static void main(String[] args){ 

        DatagramSocket socket = null; // Initialisation de la variable socket à l'extérieur du bloc try

        try {
            /////////////// PARTIE CONNEXION UDP
            InetAddress adr;
            DatagramPacket packet, packet2;
            // adr contient l'@IP de la partie serveur
            adr = InetAddress.getByName(null);
            // données à envoyer : chaîne de caractères
            byte[] data = ("youpi").getBytes();
            byte[] data2 = new byte[15];
            // création du paquet avec les données et en précisant l'adresse du serveur
            // (@IP et port sur lequel il écoute : 7777)
            packet = new DatagramPacket(data, data.length, adr, 8007);
            packet2 = new DatagramPacket(data2, data2.length);
            // création d'une socket, sans la lier à un port particulier
            socket = new DatagramSocket();

            // envoi du paquet via la socket
            socket.send(packet);
            System.out.println("packet sendt : ");
            socket.receive(packet2);
            String chaine = new String(packet2.getData(), 0, packet2.getLength()); // WARNING CHECK
            System.out.println("recu : "+chaine);
            socket.send(packet);
            System.out.println("Connexion Etabli avec le Server");
            //////////// PARTIE RMI
            // Récupération du registre RMI
            Registry registry = LocateRegistry.getRegistry("Localhost", 1099);

            // Récupération de l'objet distant à partir du registre
            ICompte compte = (ICompte) registry.lookup("CompteServer");

            // Utilisation de l'objet distant
            boolean compteCree = compte.creerCompte("pseudo", "mdp");
            if (compteCree) {
                System.out.println("Compte créé avec succès !");
            } else {
                System.out.println("Erreur lors de la création du compte.");
            }

            // Ajoutez d'autres appels de méthodes selon les besoins du client

        }catch (Exception e) {
            System.err.println("Erreur côté client : " + e.toString());
            e.printStackTrace();
        }/*catch (UnknownHostException ex) {
            Logger.getLogger(Client.class.getName()).log(Level.SEVERE, null, ex);
        } catch (SocketException ex) {
            Logger.getLogger(Client.class.getName()).log(Level.SEVERE, null, ex);
        } catch (IOException ex) {
            Logger.getLogger(Client.class.getName()).log(Level.SEVERE, null, ex);
        }*/ finally {
            // Fermeture de la socket dans le bloc finally pour garantir que la ressource est libérée
            if (socket != null) {
                socket.close();
            }
        }
    }   
    /*
    public static void main(String[] args) {
        try {            System.out.println("Balise");   
            

    }*/
}
