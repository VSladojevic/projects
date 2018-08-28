/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package control;

import javax.faces.bean.ManagedBean;
import javax.faces.bean.SessionScoped;
import beans.*;
import com.sun.xml.rpc.processor.modeler.j2ee.xml.urlPatternType;
import org.hibernate.Session;
import java.util.*;
import javax.annotation.PostConstruct;
import javax.faces.context.FacesContext;
import javax.servlet.http.HttpSession;
import org.hibernate.Query;

/**
 *
 * @author Vladimir
 */
@ManagedBean
@SessionScoped
public class Controler {

    public static User user = null;
    private HibernateUtil helper;
    private Session session;
    private String username = "";
    private String pass = "";
    public String poruka = "";

    private String chUser = "";
    private String chPass = "";
    private String chPass2 = "";

    private String chPoruka = "";

    List<Flight> flights = new ArrayList<Flight>();
    List<Flight_full> flights_full = new ArrayList<Flight_full>();

    public List<Flight_full> getFlights_full() {
        return flights_full;
    }

    public void setFlights_full(List<Flight_full> flights_full) {
        this.flights_full = flights_full;
    }

    public List<Flight> getFlights() {
        return flights;
    }

    public void setFlights(List<Flight> flights) {
        this.flights = flights;
    }

    @PostConstruct
    public void loadFlights() {
        session = helper.getSessionFactory().openSession();
        session.beginTransaction();
        //Query q = session.createQuery("from Flight");
        Query q=session.createQuery("from Flight where (planned_arrival > current_date and planned_arrival<current_date+1) or takeoff_time>current_date");
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
            /*
            Query qGrad = session.createQuery("select city from Airport where iata='" + let.getDep_airport() + "'");
            String a = qGrad.list().get(0).toString();
            f.setDep_city(a);*/

            /*
            qGrad = session.createQuery("select city from Airport where iata='" + let.getArr_airport() + "'");
            a = qGrad.list().get(0).toString();
            f.setArr_city(a);*/
            
            
            Query qPlane=session.createQuery("select M from Airplane_model M, Airplane A where A.model_id=M.id and A.id="+let.getPlane_id());
            Airplane_model a=(Airplane_model) qPlane.list().get(0);
            f.setPlane(a);
            /*
            Query qPlane = session.createQuery("select M.type from Airplane_model M, Airplane A where A.model_id=M.id and A.id=" + let.getPlane_id());
            String p = qPlane.list().get(0).toString();
            Query qPlaneCap = session.createQuery("select M.capacity from Airplane_model M, Airplane A where A.model_id=M.id and A.id=" + let.getPlane_id());

            int cap = (int) qPlaneCap.list().get(0);
            f.setPlane_type(p);
            f.setPlane_capacity(cap);
            */
            // proveriti da li je neki gate null, da li moze Integer!=null ?
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
            
            Query centers=session.createQuery("from Radar_center where flight_id="+let.getId());
            List<Radar_center> pom = (List<Radar_center>) centers.list();
            
            if(!pom.isEmpty()){
                f.setRadars(pom);
            }
            
            flights_full.add(f);
        }
    }

    public String getChPoruka() {
        return chPoruka;
    }

    public void setChPoruka(String chPoruka) {
        this.chPoruka = chPoruka;
    }

    public String LogOut() {
        FacesContext context = FacesContext.getCurrentInstance();
        HttpSession session = (HttpSession) context.getExternalContext().getSession(false);
        session.invalidate();
         try {
            FacesContext.getCurrentInstance().getExternalContext().redirect("index.xhtml");
            FacesContext.getCurrentInstance().responseComplete();
        } catch (Exception ex) {
            //Logger.getLogger(adminAirports.class.getName()).log(Level.SEVERE, null, ex);
        }
         return "index";
    }

    public User getUser() {
        return user;
    }

    public void setUser(User user) {
        this.user = user;
    }

    public String getChUser() {
        return chUser;
    }

    public void setChUser(String chUser) {
        this.chUser = chUser;
    }

    public String getChPass() {
        return chPass;
    }

    public void setChPass(String chPass) {
        this.chPass = chPass;
    }

    public String getChPass2() {
        return chPass2;
    }

    public void setChPass2(String chPass2) {
        this.chPass2 = chPass2;
    }

    public String changePass() {
        chPoruka = "";
        session = helper.getSessionFactory().openSession();
        session.beginTransaction();
        List<User> results = null;

        try {
            Query q = session.createQuery("from User where username='" + chUser + "'");
            results = (List<User>) q.list();

        } catch (Exception e) {
            e.printStackTrace();
        }
        if (results.isEmpty()) {
            chPoruka = "There is no such username";
            chUser = "";
        } else {
            Query q = session.createQuery("from User where username='" + chUser + "' and password='" + chPass + "'");
            results = (List<User>) q.list();
            if (results.isEmpty()) {
                chPoruka = "Wrong password";
            } else {
                User u = results.get(0);
                u.setPassword(chPass2);
                session.update(u);
                session.getTransaction().commit();
                return "login";
            }
        }

        return "";
    }

    public String Login() {
        poruka = "";
        session = helper.getSessionFactory().openSession();
        session.beginTransaction();
        //user = (User) session.get(User.class, 1);
        List<User> results = null;
        try {
            Query q = session.createQuery("from User where username='" + username + "'");
            results = (List<User>) q.list();

        } catch (Exception e) {
            e.printStackTrace();
        }
        if (results.isEmpty()) {
            poruka = "Wrong username";
            username = "";
        } else {
            Query q = session.createQuery("from User where username='" + username + "' and password='" + pass + "'");
            results = (List<User>) q.list();
            if (results.isEmpty()) {
                poruka = "Wrong password";
            } else {
                user = results.get(0);
                username = "";
                String redirect = "";
                if(user.getAdmin()!=0){
                    return "admin";
                }
                if (user.getApproved() != 1) {
                    poruka = "Your account hasn't been approved yet";
                    return redirect;
                }
                if (user.getType().equals("pilot")) {
                    if (user.getFirst_login() == 0) {

                        redirect = "pilot_license";
                    } else {
                        redirect = "pilot";
                    }
                } else if (user.getType().equals("worker")) {

                    redirect = "worker";
                } else {

                    redirect = "steward";
                }

                if (user.getFirst_login() == 0) {
                    user.setFirst_login(1);
                    session.update(user);
                    session.getTransaction().commit();
                }
                //poruka = "Welcome " + user.getName();
                return redirect;
            }
        }
        return "";
    }

    public String getUsername() {
        return username;
    }

    public void setUsername(String username) {
        this.username = username;
    }

    public String getPass() {
        return pass;
    }

    public void setPass(String pass) {
        this.pass = pass;
    }

    public String getPoruka() {
        return poruka;
    }

    public void setPoruka(String poruka) {
        this.poruka = poruka;
    }

}
