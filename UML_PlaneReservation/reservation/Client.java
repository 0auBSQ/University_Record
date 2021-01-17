package reservation;
import java.util.*;

public class Client {
  private String    nom;
  private List<Reservation>  reservations = new ArrayList<Reservation>();
  private String    reference;
  private String    paiement;
  private String    contact;
  private Passager  profil;

  public Client(String nom) {
    this.nom = nom;
    this.profil = new Passager(nom);
  }

  public String getName() {
    return this.nom;
  }

  public void setName(String nom) {
    this.nom = nom;
  }

  public Passager getPassager() {
    return this.profil;
  }

  public void effectue(Reservation reservation) {
    reservation.addClient(this);
    reservation.confirmer();
    reservations.add(reservation);
  }

  @Override
  public String toString() {
    return ("Client : " + this.nom);
  }
}
