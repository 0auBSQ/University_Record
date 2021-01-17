package reservation;
import java.util.*;

public class Passager {
  private String  nom;
  private List<Reservation> reservations = new ArrayList<Reservation>();

  Passager(String nom) {
    this.nom = nom;
  }

  public String getName() {
    return this.nom;
  }

  public void setName(String nom) {
    this.nom = nom;
  }

  public void addReservation(Reservation reservation) {
    reservations.add(reservation);
  }

  @Override
  public String toString() {
    return ("Passager : " + this.nom);
  }
}
