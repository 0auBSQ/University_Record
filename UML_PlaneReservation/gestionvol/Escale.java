package gestionvol;
import java.util.*;

public class Escale implements Comparable<Escale> {
  private final Date              dateDepart;
  private final Date              dateArrivee;
  private final Date              duree;
  private final Aeroport          aeroport;

  public Escale(Date dateDepart, Date dateArrivee, Aeroport aeroport) {
    this.aeroport = aeroport;
    this.dateDepart = dateDepart;
    this.dateArrivee = dateArrivee;
    this.duree = Vol.duration(dateDepart, dateArrivee);
  }

  public Date getDateDepart() {
    return this.dateDepart;
  }

  public Date getDateArrivee() {
    return this.dateArrivee;
  }

  public Date getDuree() {
    return this.duree;
  }

  @Override
  public int compareTo(Escale e) {
    return (int)(this.getDuree().getTime() - e.getDuree().getTime());
  }

  @Override
  public String toString() {
    return ("Escale : [" + Vol.dTS(this.dateDepart) + "] - [" + Vol.dTS(this.dateArrivee) + "]");
  }
}
