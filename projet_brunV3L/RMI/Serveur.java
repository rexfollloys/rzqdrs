import java.rmi.registry.LocateRegistry;
import java.rmi.registry.Registry;
import java.rmi.server.UnicastRemoteObject;
import java.util.Scanner;

//En exécutant d'abord le serveur, puis les clients, les clients peuvent créer,
//supprimer et gérer les comptes d'utilisateurs à distance grâce à RMI.
public class Serveur {

    public static void main(String[] args) {
        CompteServer compteServer = null;
        try {
            // Création de l'implémentation distante
            compteServer = new CompteServer();

            //java.rmi.server.hostname = "10.1.13.52";
            compteServer.afficherComptes(); // Afficher les comptes après initialisation

            // Exportation de l'objet distant
            ICompte stub = (ICompte) UnicastRemoteObject.exportObject(compteServer, 0);

            // Création du registre RMI
            Registry registry = LocateRegistry.createRegistry(1099);

            // Publication de l'objet distant dans le registre
            registry.rebind("CompteServer", stub);

            System.out.println("Serveur prêt !");
// Attente d'une entrée pour arrêter le serveur
            System.out.println("Appuyez sur Entrée pour arrêter le serveur.");
            Scanner scanner = new Scanner(System.in);
            scanner.nextLine();
            scanner.close();
        } catch (Exception e) {
            System.out.println("Oups");
            System.err.println("Erreur du serveur : " + e.toString());
            e.printStackTrace();
        } finally {
            // Arrêt propre du serveur
            if (compteServer != null) {
                try {
                    compteServer.sauvegarderComptesDansFichier("comptes.txt");
                    UnicastRemoteObject.unexportObject(compteServer, true);
                    System.out.println("Serveur arrêté.");
                } catch (Exception e) {
                    System.err.println("Erreur lors de l'arrêt du serveur : " + e.toString());
                    e.printStackTrace();
                }
            }
        }
    }
}
