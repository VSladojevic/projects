/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package control;

import beans.*;
import java.sql.Timestamp;
import java.util.ArrayList;
import java.util.Calendar;
import java.util.Date;
import java.util.List;
import javax.annotation.PostConstruct;
import javax.faces.bean.ManagedBean;
import javax.faces.bean.ViewScoped;
import org.hibernate.Query;
import org.hibernate.Session;

/**
 *
 * @author Vladimir
 */
@ManagedBean
@ViewScoped
public class adminFlight {

    private HibernateUtil helper;
    private Session session;
    public User user = Controler.user;
    List<Airport> airports = new ArrayList();
    List<Airplane> airplanes = new ArrayList<Airplane>();

    List<Airplane_model> plane_models = new ArrayList<Airplane_model>();
    List<Company> companies = new ArrayList<Company>();
    List<Gate> gates = new ArrayList<Gate>();

    List<Gate> depGates = new ArrayList<Gate>();
    List<Gate> arrGates = new ArrayList<Gate>();

    List<User> stewards = new ArrayList<User>();
    List<User> pilots = new ArrayList<User>();

    List<Integer> selectedStewards = new ArrayList<Integer>();

    String depIata;
    String arrIata;

    int depGateId;
    int arrGateId;
    int companyId;

    Date departure;
    Date duration;

    int planeId;
    boolean charter = false;

    int pilotId;
    int copilotId;
    int s1, s2, s3, s4, s5;

    public int getS1() {
        return s1;
    }

    public void setS1(int s1) {
        this.s1 = s1;
    }

    public int getS2() {
        return s2;
    }

    public void setS2(int s2) {
        this.s2 = s2;
    }

    public int getS3() {
        return s3;
    }

    public void setS3(int s3) {
        this.s3 = s3;
    }

    public int getS4() {
        return s4;
    }

    public void setS4(int s4) {
        this.s4 = s4;
    }

    public int getS5() {
        return s5;
    }

    public void setS5(int s5) {
        this.s5 = s5;
    }

    public List<Integer> getSelectedStewards() {
        return selectedStewards;
    }

    public void setSelectedStewards(List<Integer> selectedStewards) {
        this.selectedStewards = selectedStewards;
    }

    public int getPilotId() {
        return pilotId;
    }

    public void setPilotId(int pilotId) {
        this.pilotId = pilotId;
    }

    public int getCopilotId() {
        return copilotId;
    }

    public void setCopilotId(int copilotId) {
        this.copilotId = copilotId;
    }

    public List<User> getPilots() {
        return pilots;
    }

    public void setPilots(List<User> pilots) {
        this.pilots = pilots;
    }

    public List<User> getStewards() {
        return stewards;
    }

    public void setStewards(List<User> stewards) {
        this.stewards = stewards;
    }

    public boolean isCharter() {
        return charter;
    }

    public void setCharter(boolean charter) {
        this.charter = charter;
    }

    public String addStaff() {

        session = helper.getSessionFactory().openSession();
        session.beginTransaction();
        Flight najnoviji = (Flight) session.createQuery("from Flight ORDER BY id DESC")
                .setMaxResults(1)
                .uniqueResult();

        najnoviji.setPilot(pilotId);
        najnoviji.setCopilot(copilotId);

        if (s1 != 0) {
            najnoviji.setSteward1(s1);
        }
        if (s2 != 0) {
            najnoviji.setSteward2(s2);
        }
        if (s3 != 0) {
            najnoviji.setSteward3(s3);
        }
        if (s4 != 0) {
            najnoviji.setSteward4(s4);
        }
        if (s5 != 0) {
            najnoviji.setSteward5(s5);
        }

        session.update(najnoviji);
        session.getTransaction().commit();

        return "admin";
    }

    public String step2() {

        session = helper.getSessionFactory().openSession();
        session.beginTransaction();

        /*
        Calendar c = Calendar.getInstance();
        c.setTime(departure);
        c.add(Calendar.DATE, 7);
        */
        
        Flight f = new Flight();
        f.setDep_airport(depIata);
        f.setArr_airport(arrIata);
        f.setDep_gate(depGateId);
        f.setArr_gate(arrGateId);
        f.setPlane_id(planeId);

        java.util.Date date = departure;

        java.sql.Timestamp timestamp = new java.sql.Timestamp(date.getTime());
        f.setTakeoff_time(timestamp);

        java.sql.Time trajanje = new java.sql.Time(duration.getTime());
        f.setDuration(trajanje);

        java.sql.Timestamp t = new java.sql.Timestamp(date.getTime());
        t.setTime(t.getTime() + duration.getTime());

        f.setPlanned_arrival(t);
        f.setExpected_arrival(t);
        f.setStatus("ready");

        session.save(f);

        //
        session.getTransaction().commit();

        return "admin_flight_2";
    }

    public List<Airplane> getAirplanes() {
        return airplanes;
    }

    public void setAirplanes(List<Airplane> airplanes) {
        this.airplanes = airplanes;
    }

    public List<Airplane_model> getPlane_models() {
        return plane_models;
    }

    public void setPlane_models(List<Airplane_model> plane_models) {
        this.plane_models = plane_models;
    }

    public int getPlaneId() {
        return planeId;
    }

    public void setPlaneId(int planeId) {
        this.planeId = planeId;
    }

    public Date getDeparture() {
        return departure;
    }

    public void setDeparture(Date departure) {
        this.departure = departure;
    }

    public Date getDuration() {
        return duration;
    }

    public void setDuration(Date duration) {
        this.duration = duration;
    }

    public int getCompanyId() {
        return companyId;
    }

    public void setCompanyId(int companyId) {
        this.companyId = companyId;
    }

    public List<Company> getCompanies() {
        return companies;
    }

    public void setCompanies(List<Company> companies) {
        this.companies = companies;
    }

    public int getDepGateId() {
        return depGateId;
    }

    public void setDepGateId(int depGateId) {
        this.depGateId = depGateId;
    }

    public int getArrGateId() {
        return arrGateId;
    }

    public void setArrGateId(int arrGateId) {
        this.arrGateId = arrGateId;
    }

    public List<Gate> getDepGates() {
        return depGates;
    }

    public void setDepGates(List<Gate> depGates) {
        this.depGates = depGates;
    }

    public List<Gate> getArrGates() {
        return arrGates;
    }

    public void setArrGates(List<Gate> arrGates) {
        this.arrGates = arrGates;
    }

    public void loadDepGates() {
        session = helper.getSessionFactory().openSession();
        session.beginTransaction();
        Query q = session.createQuery("from Gate where Airport_iata='" + depIata + "'");
        depGates = (List<Gate>) q.list();
    }

    public void loadArrGates() {
        session = helper.getSessionFactory().openSession();
        session.beginTransaction();
        Query q = session.createQuery("from Gate where Airport_iata='" + arrIata + "'");
        arrGates = (List<Gate>) q.list();
    }

    public String getDepIata() {
        return depIata;
    }

    public void setDepIata(String depIata) {
        this.depIata = depIata;
    }

    public String getArrIata() {
        return arrIata;
    }

    public void setArrIata(String arrIata) {
        this.arrIata = arrIata;
    }

    public List<Gate> getGates() {
        return gates;
    }

    public void setGates(List<Gate> gates) {
        this.gates = gates;
    }

    @PostConstruct
    public void load() {
        session = helper.getSessionFactory().openSession();
        session.beginTransaction();
        Query q = session.createQuery("from Airport");
        airports = (List<Airport>) q.list();
        q = session.createQuery("from Company");
        companies = (List<Company>) q.list();

        q = session.createQuery("select plane from Airplane_model model, Airplane plane where model.id=plane.model_id order by plane.id");
        airplanes = (List<Airplane>) q.list();
        q = session.createQuery("select model from Airplane_model model, Airplane plane where model.id=plane.model_id order by plane.id");
        plane_models = (List<Airplane_model>) q.list();

        q = session.createQuery("from User where type='steward'");
        stewards = (List<User>) q.list();
        q = session.createQuery("from User where type='pilot'");
        pilots = (List<User>) q.list();

    }

    public List<Airport> getAirports() {
        return airports;
    }

    public void setAirports(List<Airport> airports) {
        this.airports = airports;
    }

    public User getUser() {
        return user;
    }

    public void setUser(User user) {
        this.user = user;
    }

}
