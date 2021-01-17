package gestionvol;
import reservation.Reservation;
import java.util.*;
import java.text.*;

public class Vol {
  private static long             current_id = 0;
  private final long              numero;
  private final Date              dateDepart;
  private final Date              dateArrivee;
  private final Date              duree;
  private final Aeroport          depart;
  private final Aeroport          arrivee;
  private List<Escale>            escales = new ArrayList<Escale>();
  private List<Reservation>       reservations = new ArrayList<Reservation>();
  private Companie                companie;
  private boolean                 ouvert;

  public Vol(Date dateDepart, Date dateArrivee, int nb_places, Aeroport depart, Aeroport arrivee) {
    if (nb_places < 1) {
      throw new IllegalArgumentException("Un Vol a besoin d'au moins une place");
    }
    this.numero = Vol.generateNumber();
    for (int i = 0; i < nb_places; i++) {
      this.reservations.add(new Reservation(dateDepart));
    }
    this.reservations = reservations;
    this.dateDepart = dateDepart;
    this.dateArrivee = dateArrivee;
    this.depart = depart;
    this.arrivee = arrivee;
    depart.attachVol(this);
    arrivee.attachVol(this);
    this.duree = Vol.duration(dateDepart, dateArrivee);
    this.ouvert = true;
  }

  public static long generateNumber() {
    Vol.current_id += 1;
    return (Vol.current_id);
  }

  public static String dTS(Date d) {
    DateFormat df = new SimpleDateFormat("dd/MM/yyyy HH:mm:ss");
    return (df.format(d));
  }

  public static Date duration(Date d, Date a) {
    return (new Date(a.getTime() - d.getTime()));
  }

  public void addEscale(Escale e) {
    if (this.escales.contains(e) == false) {
      this.escales.add(e);
      Collections.sort(this.escales);
    }
  }

  public void attachCompanie(Companie c) {
    this.companie = c;
  }

  public List<Reservation> getReservations() {
    return this.reservations;
  }

  public Reservation reservationInfo(int i) {
    if (i >= this.reservations.size() || i < 0) {
      throw new IllegalArgumentException("Reservation demandée non existante");
    }
    return this.reservations.get(i);
  }

  public long getNumber() {
    return this.numero;
  }

  public Aeroport getDepart() {
    return this.depart;
  }

  public Aeroport getArrivee() {
    return this.arrivee;
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

  public void ouvrir() {
    this.ouvert = true;
  }

  public void fermer() {
    this.ouvert = false;
  }

  @Override
  public String toString() {
    return ("Vol " + this.numero + " (" + this.companie + ") : [" + Vol.dTS(this.dateDepart) + "] - [" + Vol.dTS(this.dateArrivee) + "] [" + this.reservations.size() + " reservations] [" + this.depart + " - " + this.arrivee + "]" + " [" + this.escales.size() + " escales" + "]" );
  }
}
