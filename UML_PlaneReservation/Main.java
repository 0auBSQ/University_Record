import reservation.*;
import gestionvol.*;
import java.util.*;

public class Main {
  public static void main(String args[]) {
    List<Reservation> r = new ArrayList<Reservation>();
    r.add(new Reservation(new Date()));
    r.add(new Reservation(new Date()));
    r.add(new Reservation(new Date()));

    Ville v = new Ville("Paris");
    Aeroport a = new Aeroport("CDG", v);
    Aeroport b = new Aeroport("Orly", v);
    Ville w = new Ville("Moscou");
    Aeroport c = new Aeroport("Vodka", w);
    Escale e = new Escale(new Date(), new Date(), b);
    Companie co = new Companie("Air France");

    Vol k = new Vol(new Date(), new Date(), 500, a, c);
    k.addEscale(e);
    Vol l = new Vol(new Date(), new Date(), 35, b, c);

    Client cl = new Client("Kevin");
    cl.effectue(k.getReservations().get(1));

    co.propose(k);
    co.propose(l);

    for (Reservation rt : r) {
      System.out.println(rt);
    }
    System.out.println(k.reservationInfo(0));
    System.out.println(k.reservationInfo(1));
    System.out.println(k.reservationInfo(2));
    System.out.println(cl);
    System.out.println(v);
    System.out.println(a);
    System.out.println(co);
    System.out.println(k);
    System.out.println(l);
  }

}
