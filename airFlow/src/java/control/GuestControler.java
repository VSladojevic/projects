/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package control;

import beans.Airplane_model;
import beans.Airport;
import beans.Company;
import beans.Flight;
import beans.Flight_full;
import beans.Flight_result;
import beans.HibernateUtil;
import beans.Radar_center;
import beans.Reservation;
import java.text.SimpleDateFormat;
import java.util.Calendar;
import java.util.Date;
import javax.faces.application.FacesMessage;
import javax.faces.bean.ManagedBean;
import javax.faces.context.FacesContext;

import org.primefaces.context.RequestContext;
import org.primefaces.event.SelectEvent;
import control.Controler;
import static control.RegBean.kompanije;
import javax.faces.bean.ViewScoped;
import javax.faces.bean.SessionScoped;
import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.ThreadLocalRandom;
import javax.annotation.PostConstruct;
import org.hibernate.Query;
import org.hibernate.Session;

/**
 *
 * @author Vladimir
 */
@ManagedBean
@SessionScoped
public class GuestControler {

    Date departure;
    Date back;
    boolean povratna = false;

    Date today = new Date();
    Date minReturn = new Date();

    int adults = 1;

    String from, to;
    boolean direktan = false;

    Flight_result toRes;

    String poruka = "";

    private List<Flight_result> results = new ArrayList<Flight_result>();

    private List<Flight_result> resultsReturn = new ArrayList<Flight_result>();

    static List<Airport> airports;
    private static HibernateUtil helper;
    private static Session session;
    static List<String> airports_IATA;

    String name, surname, card;

    public Flight_result getToRes() {
        return toRes;
    }

    public void setToRes(Flight_result toRes) {
        this.toRes = toRes;
    }

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    public String getSurname() {
        return surname;
    }

    public void setSurname(String surname) {
        this.surname = surname;
    }

    public String getCard() {
        return card;
    }

    public void setCard(String card) {
        this.card = card;
    }

    static {
        airports = new ArrayList<Airport>();
        airports_IATA = new ArrayList<>();
        session = helper.getSessionFactory().openSession();
        session.beginTransaction();

        List<Airport> results = null;
        try {
            Query q = session.createQuery("from Airport");
            results = (List<Airport>) q.list();

        } catch (Exception e) {
            e.printStackTrace();
        }
        if (results.isEmpty()) {
            airports = null;
        } else {
            airports = results;
            for (Airport a : airports) {
                airports_IATA.add(a.getCity() + "(" + a.getIata() + ")");
            }
        }
    }

    public List<String> fromComplete(String query) {
        List<String> results = new ArrayList<String>();
        for (String s : airports_IATA) {
            if (query.equals("") || query.equals(" ")) {
                results.add(s);
            } else if (s.toLowerCase().contains(query.toLowerCase())) {
                results.add(s);
            }
        }

        return results;
    }

    public String search() {
        if (direktan) {
            if (!povratna) {
                searchDirectOneway();
            } else {
                searchDirectReturn();
            }
        } else {
            if (!povratna) {
                searchAllOneway();
            } else {
                searchAllReturn();
            }
        }
        return "guest_results";
    }

    public void onDateSelect(SelectEvent event) {
        FacesContext facesContext = FacesContext.getCurrentInstance();
        SimpleDateFormat format = new SimpleDateFormat("dd/MM/yyyy");
        facesContext.addMessage(null, new FacesMessage(FacesMessage.SEVERITY_INFO, "Date Selected", format.format(event.getObject())));
    }

    public void click() {
        RequestContext requestContext = RequestContext.getCurrentInstance();

        requestContext.update("form:display");
        requestContext.execute("PF('dlg').show()");
    }

    public String getFrom() {
        return from;
    }

    public void setFrom(String from) {
        this.from = from;
    }

    public String getTo() {
        return to;
    }

    public void setTo(String to) {
        this.to = to;
    }

    public int getAdults() {
        return adults;
    }

    public void setAdults(int adults) {
        this.adults = adults;
    }

    public boolean isDirektan() {
        return direktan;
    }

    public void setDirektan(boolean direktan) {
        this.direktan = direktan;
    }

    public String getPoruka() {
        return poruka;
    }

    public void setPoruka(String poruka) {
        this.poruka = poruka;
    }

    public Date getDeparture() {
        return departure;
    }

    public void setDeparture(Date departure) {
        this.departure = departure;
    }

    public boolean isPovratna() {
        return povratna;
    }

    public void setPovratna(boolean showR) {
        this.povratna = showR;
    }

    public Date getBack() {
        return back;
    }

    public void setBack(Date back) {
        this.back = back;
    }

    public Date getToday() {
        return today;
    }

    public void setToday(Date today) {
        this.today = today;
    }

    public List<Flight_result> getResultsReturn() {
        return resultsReturn;
    }

    public void setResultsReturn(List<Flight_result> resultsReturn) {
        this.resultsReturn = resultsReturn;
    }

    public List<Flight_result> getResults() {
        return results;
    }

    public void setResults(List<Flight_result> results) {
        this.results = results;
    }

    public Date getMinReturn() {
        return minReturn;
    }

    public void setMinReturn(Date minReturn) {
        this.minReturn = minReturn;
    }

    private void searchDirectOneway() {

        session = helper.getSessionFactory().openSession();
        session.beginTransaction();
        String fromIata = from.substring(from.length() - 4, from.length() - 1);

        String toIata = to.substring(to.length() - 4, to.length() - 1);

        java.sql.Date sqlDate = new java.sql.Date(departure.getTime());

        //Query q = session.createQuery("from Flight");
        Query q = session.createQuery("from Flight where Dep_airport='" + fromIata + "' and Arr_airport='" + toIata + "' and Takeoff_time between '" + sqlDate + "' and '" + sqlDate + " 23:59:59" + "'");
        List<Flight> flights = (List<Flight>) q.list();
        for (Flight let : flights) {
            Flight_full f = new Flight_full(let);
            // settovanje svega
            Query dep = session.createQuery("from Airport where iata='" + let.getDep_airport() + "'");
            Airport tmp = (Airport) dep.list().get(0);
            f.setDep(tmp);

            Query arr = session.createQuery("from Airport where iata='" + let.getArr_airport() + "'");
            Airport tmp2 = (Airport) arr.list().get(0);
            f.setArr(tmp2);

            Query qPlane = session.createQuery("select M from Airplane_model M, Airplane A where A.model_id=M.id and A.id=" + let.getPlane_id());
            Airplane_model a = (Airplane_model) qPlane.list().get(0);
            f.setPlane(a);

            if (let.getDep_gate() != null) {
                Query qGate = session.createQuery("select name from Gate where id=" + let.getDep_gate());
                String aa = qGate.list().get(0).toString();
                f.setDep_gate_name(aa);
            }
            if (let.getArr_gate() != null) {
                Query qGate1 = session.createQuery("select name from Gate where id=" + let.getArr_gate());
                String bb = qGate1.list().get(0).toString();
                f.setArr_gate_name(bb);
            }

            Query centers = session.createQuery("from Radar_center where flight_id=" + let.getId());
            List<Radar_center> pom = (List<Radar_center>) centers.list();

            if (!pom.isEmpty()) {
                f.setRadars(pom);
            }

            results.add(new Flight_result(f));
        }

    }

    private void searchDirectReturn() {
        searchDirectOneway();
        session = helper.getSessionFactory().openSession();
        session.beginTransaction();
        String fromIata
                = to.substring(to.length() - 4, to.length() - 1);

        String toIata = from.substring(from.length() - 4, from.length() - 1);

        java.sql.Date sqlDate = new java.sql.Date(back.getTime());

        //Query q = session.createQuery("from Flight");
        Query q = session.createQuery("from Flight where Dep_airport='" + fromIata + "' and Arr_airport='" + toIata + "' and Takeoff_time between '" + sqlDate + "' and '" + sqlDate + " 23:59:59" + "'");
        List<Flight> flights = (List<Flight>) q.list();
        for (Flight let : flights) {
            Flight_full f = new Flight_full(let);
            // settovanje svega
            Query dep = session.createQuery("from Airport where iata='" + let.getDep_airport() + "'");
            Airport tmp = (Airport) dep.list().get(0);
            f.setDep(tmp);

            Query arr = session.createQuery("from Airport where iata='" + let.getArr_airport() + "'");
            Airport tmp2 = (Airport) arr.list().get(0);
            f.setArr(tmp2);

            Query qPlane = session.createQuery("select M from Airplane_model M, Airplane A where A.model_id=M.id and A.id=" + let.getPlane_id());
            Airplane_model a = (Airplane_model) qPlane.list().get(0);
            f.setPlane(a);

            if (let.getDep_gate() != null) {
                Query qGate = session.createQuery("select name from Gate where id=" + let.getDep_gate());
                String aa = qGate.list().get(0).toString();
                f.setDep_gate_name(aa);
            }
            if (let.getArr_gate() != null) {
                Query qGate1 = session.createQuery("select name from Gate where id=" + let.getArr_gate());
                String bb = qGate1.list().get(0).toString();
                f.setArr_gate_name(bb);
            }

            Query centers = session.createQuery("from Radar_center where flight_id=" + let.getId());
            List<Radar_center> pom = (List<Radar_center>) centers.list();

            if (!pom.isEmpty()) {
                f.setRadars(pom);
            }

            resultsReturn.add(new Flight_result(f));
        }
    }

    private void searchAllReturn() {
        searchAllOneway();

        session = helper.getSessionFactory().openSession();
        session.beginTransaction();
        String fromIata = to.substring(to.length() - 4, to.length() - 1);

        String toIata = from.substring(from.length() - 4, from.length() - 1);

        java.sql.Date sqlDate = new java.sql.Date(back.getTime());

        //Query q = session.createQuery("from Flight");
        Query q = session.createQuery("from Flight where Dep_airport='" + fromIata + "' and Arr_airport='" + toIata + "' and Takeoff_time between '" + sqlDate + "' and '" + sqlDate + " 23:59:59" + "'");
        List<Flight> flights = (List<Flight>) q.list();
        for (Flight let : flights) {
            Flight_full f = new Flight_full(let);
            // settovanje svega
            Query dep = session.createQuery("from Airport where iata='" + let.getDep_airport() + "'");
            Airport tmp = (Airport) dep.list().get(0);
            f.setDep(tmp);

            Query arr = session.createQuery("from Airport where iata='" + let.getArr_airport() + "'");
            Airport tmp2 = (Airport) arr.list().get(0);
            f.setArr(tmp2);

            Query qPlane = session.createQuery("select M from Airplane_model M, Airplane A where A.model_id=M.id and A.id=" + let.getPlane_id());
            Airplane_model a = (Airplane_model) qPlane.list().get(0);
            f.setPlane(a);

            if (let.getDep_gate() != null) {
                Query qGate = session.createQuery("select name from Gate where id=" + let.getDep_gate());
                String aa = qGate.list().get(0).toString();
                f.setDep_gate_name(aa);
            }
            if (let.getArr_gate() != null) {
                Query qGate1 = session.createQuery("select name from Gate where id=" + let.getArr_gate());
                String bb = qGate1.list().get(0).toString();
                f.setArr_gate_name(bb);
            }

            Query centers = session.createQuery("from Radar_center where flight_id=" + let.getId());
            List<Radar_center> pom = (List<Radar_center>) centers.list();

            if (!pom.isEmpty()) {
                f.setRadars(pom);
            }

            resultsReturn.add(new Flight_result(f));
        }

        Query q1 = session.createQuery("from Flight where Dep_airport='" + fromIata + "' and Takeoff_time between '" + sqlDate + "' and '" + sqlDate + " 23:59:59" + "'");
        // iz polaznog grada
        List<Flight> flights1 = (List<Flight>) q1.list();
        for (Flight pom : flights1) {

            Query q2 = session.createQuery("from Flight where Dep_airport='" + pom.getArr_airport() + "' and Arr_airport='" + toIata + "' and Takeoff_time>'" + pom.getPlanned_arrival() + "'");
            List<Flight> flights2 = (List<Flight>) q2.list();
            if (!flights2.isEmpty()) {
                Flight let = pom;
                Flight_full f = new Flight_full(let);
                // settovanje svega
                Query dep = session.createQuery("from Airport where iata='" + let.getDep_airport() + "'");
                Airport tmp = (Airport) dep.list().get(0);
                f.setDep(tmp);

                Query arr = session.createQuery("from Airport where iata='" + let.getArr_airport() + "'");
                Airport tmp2 = (Airport) arr.list().get(0);
                f.setArr(tmp2);

                Query qPlane = session.createQuery("select M from Airplane_model M, Airplane A where A.model_id=M.id and A.id=" + let.getPlane_id());
                Airplane_model a = (Airplane_model) qPlane.list().get(0);
                f.setPlane(a);

                if (let.getDep_gate() != null) {
                    Query qGate = session.createQuery("select name from Gate where id=" + let.getDep_gate());
                    String aa = qGate.list().get(0).toString();
                    f.setDep_gate_name(aa);
                }
                if (let.getArr_gate() != null) {
                    Query qGate1 = session.createQuery("select name from Gate where id=" + let.getArr_gate());
                    String bb = qGate1.list().get(0).toString();
                    f.setArr_gate_name(bb);
                }

                Query centers = session.createQuery("from Radar_center where flight_id=" + let.getId());
                List<Radar_center> pom1 = (List<Radar_center>) centers.list();

                if (!pom1.isEmpty()) {
                    f.setRadars(pom1);
                }

                for (Flight flight : flights2) {
                    Flight_full ff = new Flight_full(flight);
                    // settovanje svega
                    Query depf = session.createQuery("from Airport where iata='" + flight.getDep_airport() + "'");
                    Airport tmpf = (Airport) depf.list().get(0);
                    ff.setDep(tmpf);

                    Query arrf = session.createQuery("from Airport where iata='" + flight.getArr_airport() + "'");
                    Airport tmpf2 = (Airport) arrf.list().get(0);
                    ff.setArr(tmpf2);

                    Query qPlanef = session.createQuery("select M from Airplane_model M, Airplane A where A.model_id=M.id and A.id=" + flight.getPlane_id());
                    Airplane_model af = (Airplane_model) qPlanef.list().get(0);
                    ff.setPlane(af);

                    if (flight.getDep_gate() != null) {
                        Query qGate = session.createQuery("select name from Gate where id=" + flight.getDep_gate());
                        String aa = qGate.list().get(0).toString();
                        ff.setDep_gate_name(aa);
                    }
                    if (flight.getArr_gate() != null) {
                        Query qGate1 = session.createQuery("select name from Gate where id=" + flight.getArr_gate());
                        String bb = qGate1.list().get(0).toString();
                        ff.setArr_gate_name(bb);
                    }

                    Query centersf = session.createQuery("from Radar_center where flight_id=" + flight.getId());
                    List<Radar_center> pom2 = (List<Radar_center>) centersf.list();

                    if (!pom2.isEmpty()) {
                        ff.setRadars(pom2);
                    }

                    resultsReturn.add(new Flight_result(f, ff));
                }

            }

        }

    }

    private void searchAllOneway() {

        session = helper.getSessionFactory().openSession();
        session.beginTransaction();
        String fromIata = from.substring(from.length() - 4, from.length() - 1);

        String toIata = to.substring(to.length() - 4, to.length() - 1);

        java.sql.Date sqlDate = new java.sql.Date(departure.getTime());

        //Query q = session.createQuery("from Flight");
        Query q = session.createQuery("from Flight where Dep_airport='" + fromIata + "' and Arr_airport='" + toIata + "' and Takeoff_time between '" + sqlDate + "' and '" + sqlDate + " 23:59:59" + "'");
        List<Flight> flights = (List<Flight>) q.list();
        for (Flight let : flights) {
            Flight_full f = new Flight_full(let);
            // settovanje svega
            Query dep = session.createQuery("from Airport where iata='" + let.getDep_airport() + "'");
            Airport tmp = (Airport) dep.list().get(0);
            f.setDep(tmp);

            Query arr = session.createQuery("from Airport where iata='" + let.getArr_airport() + "'");
            Airport tmp2 = (Airport) arr.list().get(0);
            f.setArr(tmp2);

            Query qPlane = session.createQuery("select M from Airplane_model M, Airplane A where A.model_id=M.id and A.id=" + let.getPlane_id());
            Airplane_model a = (Airplane_model) qPlane.list().get(0);
            f.setPlane(a);

            if (let.getDep_gate() != null) {
                Query qGate = session.createQuery("select name from Gate where id=" + let.getDep_gate());
                String aa = qGate.list().get(0).toString();
                f.setDep_gate_name(aa);
            }
            if (let.getArr_gate() != null) {
                Query qGate1 = session.createQuery("select name from Gate where id=" + let.getArr_gate());
                String bb = qGate1.list().get(0).toString();
                f.setArr_gate_name(bb);
            }

            Query centers = session.createQuery("from Radar_center where flight_id=" + let.getId());
            List<Radar_center> pom = (List<Radar_center>) centers.list();

            if (!pom.isEmpty()) {
                f.setRadars(pom);
            }

            results.add(new Flight_result(f));
        }

        Query q1 = session.createQuery("from Flight where Dep_airport='" + fromIata + "' and Takeoff_time between '" + sqlDate + "' and '" + sqlDate + " 23:59:59" + "'");
        // iz polaznog grada
        List<Flight> flights1 = (List<Flight>) q1.list();
        for (Flight pom : flights1) {

            Query q2 = session.createQuery("from Flight where Dep_airport='" + pom.getArr_airport() + "' and Arr_airport='" + toIata + "' and Takeoff_time>'" + pom.getPlanned_arrival() + "'");
            List<Flight> flights2 = (List<Flight>) q2.list();
            if (!flights2.isEmpty()) {
                Flight let = pom;
                Flight_full f = new Flight_full(let);
                // settovanje svega
                Query dep = session.createQuery("from Airport where iata='" + let.getDep_airport() + "'");
                Airport tmp = (Airport) dep.list().get(0);
                f.setDep(tmp);

                Query arr = session.createQuery("from Airport where iata='" + let.getArr_airport() + "'");
                Airport tmp2 = (Airport) arr.list().get(0);
                f.setArr(tmp2);

                Query qPlane = session.createQuery("select M from Airplane_model M, Airplane A where A.model_id=M.id and A.id=" + let.getPlane_id());
                Airplane_model a = (Airplane_model) qPlane.list().get(0);
                f.setPlane(a);

                if (let.getDep_gate() != null) {
                    Query qGate = session.createQuery("select name from Gate where id=" + let.getDep_gate());
                    String aa = qGate.list().get(0).toString();
                    f.setDep_gate_name(aa);
                }
                if (let.getArr_gate() != null) {
                    Query qGate1 = session.createQuery("select name from Gate where id=" + let.getArr_gate());
                    String bb = qGate1.list().get(0).toString();
                    f.setArr_gate_name(bb);
                }

                Query centers = session.createQuery("from Radar_center where flight_id=" + let.getId());
                List<Radar_center> pom1 = (List<Radar_center>) centers.list();

                if (!pom1.isEmpty()) {
                    f.setRadars(pom1);
                }

                for (Flight flight : flights2) {
                    Flight_full ff = new Flight_full(flight);
                    // settovanje svega
                    Query depf = session.createQuery("from Airport where iata='" + flight.getDep_airport() + "'");
                    Airport tmpf = (Airport) depf.list().get(0);
                    ff.setDep(tmpf);

                    Query arrf = session.createQuery("from Airport where iata='" + flight.getArr_airport() + "'");
                    Airport tmpf2 = (Airport) arrf.list().get(0);
                    ff.setArr(tmpf2);

                    Query qPlanef = session.createQuery("select M from Airplane_model M, Airplane A where A.model_id=M.id and A.id=" + flight.getPlane_id());
                    Airplane_model af = (Airplane_model) qPlanef.list().get(0);
                    ff.setPlane(af);

                    if (flight.getDep_gate() != null) {
                        Query qGate = session.createQuery("select name from Gate where id=" + flight.getDep_gate());
                        String aa = qGate.list().get(0).toString();
                        ff.setDep_gate_name(aa);
                    }
                    if (flight.getArr_gate() != null) {
                        Query qGate1 = session.createQuery("select name from Gate where id=" + flight.getArr_gate());
                        String bb = qGate1.list().get(0).toString();
                        ff.setArr_gate_name(bb);
                    }

                    Query centersf = session.createQuery("from Radar_center where flight_id=" + flight.getId());
                    List<Radar_center> pom2 = (List<Radar_center>) centersf.list();

                    if (!pom2.isEmpty()) {
                        ff.setRadars(pom2);
                    }

                    results.add(new Flight_result(f, ff));
                }

            }

        }
    }

    public String reserve(Flight_result res) {

        toRes = res;
        return "reservation";
    }

    public String confirmReservation() {

        session = helper.getSessionFactory().openSession();
        session.beginTransaction();

        int id1 = toRes.getF1().getBasic().getId();
        int id2 = 0;

        Reservation rez = new Reservation();
        int randomNum = ThreadLocalRandom.current().nextInt(10000000, 99999999);
        rez.setCode(randomNum);
        rez.setName(name);
        rez.setSurname(surname);
        rez.setFlight_id(id1);
        if (toRes.getF2() != null) {
            id2 = toRes.getF2().getBasic().getId();
            rez.setFlight_id2(id2);
        }

        Flight f1 = (Flight) session.get(Flight.class, id1);
        f1.setReserved_seats(f1.getReserved_seats() + adults);
        session.update(f1);

        if (id2 != 0) {
            Flight f2 = (Flight) session.get(Flight.class, id2);
            f2.setReserved_seats(f2.getReserved_seats() + adults);
            session.update(f2);
        }

        session.save(rez);

        session.getTransaction().commit();

        return "guest_results";
    }

}
