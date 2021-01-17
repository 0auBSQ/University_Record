package gestionvol;
import java.util.*;

public class Ville {
  private String            nom;
  private List<Aeroport>    aeroports = new ArrayList<Aeroport>();

  public Ville(String nom) {
    this.nom = nom;
  }

  public String getName() {
    return this.nom;
  }

  public void setName(String nom) {
    this.nom = nom;
  }

  public void attachAeroport(Aeroport a) {
    this.aeroports.add(a);
  }

  @Override
  public String toString() {
    return ("Ville : " + this.nom);
  }
}
