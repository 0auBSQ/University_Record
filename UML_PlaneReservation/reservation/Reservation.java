package reservation;
import gestionvol.Vol;
import java.util.*;

public class Reservation {
  private static long     current_id = 0;
  private long            numero;
  private Vol             reference;
  private Date            date;
  private Passager        passager;
  private boolean         active;

  public Reservation(Date date) {
    this.numero = Reservation.generateNumber();
    this.date = date;
    this.active = false;
  }

  public static long generateNumber() {
    Reservation.current_id += 1;
    return (Reservation.current_id);
  }

  public void setReference(Vol v) {
    this.reference = v;
  }

  public void confirmer() {
    this.active = true;
  }

  public void annuler() {
    this.active = false;
  }

  public long getNumber() {
    return this.numero;
  }

  public Vol getReference() {
    return this.reference;
  }

  public Date getDate() {
    return this.date;
  }

  public void setDate(Date date) {
    this.date = date;
  }

  public void addClient(Client c) {
    this.passager = c.getPassager();
    this.passager.addReservation(this);
  }

  @Override
  public String toString() {
    return ("Reservation " + this.numero + " : [" + Vol.dTS(this.date) + "] [Active : " + this.active + "]" );
  }
}
