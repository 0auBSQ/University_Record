package gestionvol;
import java.util.*;

public class Aeroport {
  private String      nom;
  private List<Vol>   vols = new ArrayList<Vol>();
  private List<Ville> villes = new ArrayList<Ville>();

  public Aeroport(String nom, Ville ville) {
    this.nom = nom;
    this.villes.add(ville);
  }

  public String getName() {
    return this.nom;
  }

  public void setName(String nom) {
    this.nom = nom;
  }

  public void attachVol(Vol v) {
    this.vols.add(v);
  }

  public void attachVille(Ville v) {
    v.attachAeroport(this);
    this.villes.add(v);
  }

  @Override
  public String toString() {
    return ("Aeroport : " + this.nom);
  }
}
