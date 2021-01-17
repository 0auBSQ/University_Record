package gestionvol;
import java.util.*;

public class Companie {
  private String            nom;
  private final List<Vol>   vols = new ArrayList<Vol>();

  public Companie(String nom) {
    this.nom = nom;
  }

  public String getName() {
    return this.nom;
  }

  public void propose(Vol v) {
    v.attachCompanie(this);
    vols.add(v);
  }

  // Replanifie un vol a des dates données différentes, pour définir des vols réguliers
  public void reschedule(Vol v, Date d, Date a) {
    if (this.vols.contains(v) == true) {
      this.vols.add(new Vol(d, a, v.getReservations().size(), v.getDepart(), v.getArrivee()));
    }
  }

  public void setName(String nom) {
    this.nom = nom;
  }

  @Override
  public String toString() {
    return ("Companie : " + this.nom + " [" + this.vols.size() + " vols]");
  }
}
