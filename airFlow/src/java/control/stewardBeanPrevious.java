/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package control;

import beans.*;
import java.util.ArrayList;
import java.util.List;
import javax.annotation.PostConstruct;
import javax.faces.bean.ManagedBean;
import javax.faces.bean.SessionScoped;
import javax.faces.bean.ViewScoped;
import org.hibernate.Query;
import org.hibernate.Session;

/**
 *
 * @author Vladimir
 */
@ManagedBean
@ViewScoped
public class stewardBeanPrevious {

    public User user = Controler.user;
    private HibernateUtil helper;
    private Session session;
   

    List<Flight> flights = new ArrayList<Flight>();
    List<Flight_full> flights_full = new ArrayList<Flight_full>();

   
    public User getUser() {
        return user;
    }

    public void setUser(User user) {
        this.user = user;
    }

    public List<Flight> getFlights() {
        return flights;
    }

    public void setFlights(List<Flight> flights) {
        this.flights = flights;
    }

    public List<Flight_full> getFlights_full() {
        return flights_full;
    }

    public void setFlights_full(List<Flight_full> flights_full) {
        this.flights_full = flights_full;
    }
    

    @PostConstruct
    public void loadFlights() {
        session = helper.getSessionFactory().openSession();
        session.beginTransaction();
        //Query q = session.createQuery("from Flight");
        Query q = session.createQuery("from Flight where (Steward1=" + user.getId() + " or Steward2=" + user.getId() + " or Steward3=" + user.getId() + " or Steward4=" + user.getId() + " or Steward5=" + user.getId()+") and takeoff_time<current_date");
        flights = (List<Flight>) q.list();
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

            flights_full.add(f);
        }

    }

}
