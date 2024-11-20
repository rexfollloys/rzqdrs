import java.rmi.RemoteException;
import java.rmi.registry.LocateRegistry;
import java.rmi.registry.Registry;
import java.rmi.server.UnicastRemoteObject;
import java.util.HashMap;
import java.util.Map;
import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import java.io.BufferedWriter;
import java.io.FileWriter;


public class CompteServer implements ICompte {

    private Map<String, String> comptes; // stockage des pseudos et mots de passe

    public CompteServer() {
        comptes = new HashMap<>();
        initialiserComptesDepuisFichier("comptes.txt"); // Initialiser les comptes depuis le fichier
    }

    public void afficherComptes() {
        System.out.println("Contenu de la Map comptes :");
        for (Map.Entry<String, String> entry : comptes.entrySet()) {
            System.out.println("Pseudo : " + entry.getKey() + ", Mot de passe : " + entry.getValue());
        }
    }

    @Override // doit remplacer ou implémenter la méthode de la classe parente.
    public boolean creerCompte(String pseudo, String mdp) throws RemoteException {
        if (!comptes.containsKey(pseudo)) {
            comptes.put(pseudo, mdp);
            return true;
        } else {
            return false;
        }
    }

    @Override
    public boolean supprimerCompte(String pseudo, String mdp) throws RemoteException {
        /*vérifie simultanément si le pseudo existe dans la Map comptes et si le mot de 
        passe associé à ce pseudo est égal au mot de passe fourni en paramètre.*/
        if (comptes.containsKey(pseudo) && comptes.get(pseudo).equals(mdp)) {
            comptes.remove(pseudo);
            return true;
        } else {
            return false;
        }
    }

    @Override
    public boolean connexion(String pseudo, String mdp) throws RemoteException {
        return comptes.containsKey(pseudo) && comptes.get(pseudo).equals(mdp);
    }

    private void initialiserComptesDepuisFichier(String nomFichier) {
        try (BufferedReader br = new BufferedReader(new FileReader(nomFichier))) {
            String ligne;
            while ((ligne = br.readLine()) != null) {
                String[] parts = ligne.split("/", 2);
                if (parts.length == 2) {
                    comptes.put(parts[0], parts[1]);
                }
            }
        } catch (IOException e) {
            System.out.println("Oups");
            e.printStackTrace();
        }
    }

    public void sauvegarderComptesDansFichier(String nomFichier) {
        try (BufferedWriter writer = new BufferedWriter(new FileWriter(nomFichier))) {
            for (Map.Entry<String, String> entry : comptes.entrySet()) {
                writer.write(entry.getKey() + "/" + entry.getValue());
                writer.newLine(); // Nouvelle ligne pour chaque paire pseudo:mdp
            }
            System.out.println("Comptes sauvegardés dans le fichier " + nomFichier);
        } catch (IOException e) {
            System.err.println("Erreur lors de l'écriture dans le fichier " + nomFichier + ": " + e.getMessage());
        }
    }

    public static void main(String[] args) {
        try {
            // Création et exportation de l'objet serveur
            CompteServer compteServer = new CompteServer();
            
            ICompte interfaceDistante = (ICompte) UnicastRemoteObject.exportObject(compteServer, 0);

            // Liaison de l'objet avec le registre RMI
            Registry registry = LocateRegistry.getRegistry();
            registry.rebind("CompteServer", interfaceDistante);

            System.out.println("Serveur prêt !");
        } catch (Exception e) {
            System.out.println("Oups");
            System.err.println("Erreur du serveur : " + e.toString());
            e.printStackTrace();
        }
    }
}
