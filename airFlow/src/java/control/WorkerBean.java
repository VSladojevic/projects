 /*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package control;

import javax.faces.bean.ManagedBean;
import javax.faces.bean.SessionScoped;
import beans.*;
import java.util.ArrayList;
import java.util.List;
import javax.annotation.PostConstruct;
import javax.faces.context.FacesContext;
import org.hibernate.Query;
import org.hibernate.Session;
import org.primefaces.event.SelectEvent;
import org.primefaces.model.map.DefaultMapModel;
import org.primefaces.model.map.LatLng;
import org.primefaces.model.map.MapModel;
import org.primefaces.model.map.Marker;
import org.primefaces.model.map.Polyline;

/**
 *
 * @author Vladimir
 */
@ManagedBean
@SessionScoped
public class WorkerBean {

    public User user = Controler.user;
    private HibernateUtil helper;
    private Session session;
    private List<Airplane_model> models = new ArrayList<Airplane_model>();
    private List<Airplane> planes = new ArrayList<Airplane>();
    private Flight_full selected_flight;

    private MapModel simpleModel;

    private void inicMap() {
        simpleModel = new DefaultMapModel();

        //Shared coordinates
        LatLng coord1 = new LatLng(selected_flight.getDep().getLatitude(), selected_flight.getDep().getLongitude());
        LatLng coord2 = new LatLng(selected_flight.getArr().getLatitude(), selected_flight.getArr().getLongitude());

        Marker m1 = new Marker(coord1, selected_flight.getDep().getIata());
        Marker m2 = new Marker(coord2, selected_flight.getArr().getIata());
        m1.setIcon("faces/resources/icon_green.png");
        m2.setIcon("faces/resources/icon_red.png");
        //Basic marker
        simpleModel.addOverlay(m1);
        simpleModel.addOverlay(m2);

        Polyline polyline = new Polyline();
        polyline.getPaths().add(coord1);

        List<Marker> rcs = new ArrayList<Marker>();

        if (selected_flight.getRadars().size() > 0) {
            for (Radar_center r : selected_flight.getRadars()) {
                LatLng coord = new LatLng(r.getLatitude(), r.getLongitude());
                Marker m = new Marker(coord);
                m.setIcon("faces/resources/icon_blue.png");
                rcs.add(m);
                simpleModel.addOverlay(m);
                polyline.getPaths().add(coord);
            }
        }
        
        polyline.getPaths().add(coord2);
        
        
        polyline.setStrokeWeight(4);
        polyline.setStrokeColor("#FF9900");
        polyline.setStrokeOpacity(0.7);

        simpleModel.addOverlay(polyline);
    }

    public MapModel getSimpleModel() {
        return simpleModel;
    }

    public void setSimpleModel(MapModel simpleModel) {
        this.simpleModel = simpleModel;
    }

    public Flight_full getSelected_flight() {
        return selected_flight;
    }

    public void setSelected_flight(Flight_full selected_flight) {
        this.selected_flight = selected_flight;
    }

    public User getUser() {
        return user;
    }

    public void setUser(User user) {
        this.user = user;
    }

    public void onRowSelect() {
        try {
            inicMap();
            FacesContext.getCurrentInstance().getExternalContext().redirect("worker_flight_details.xhtml");

        } catch (Exception e) {
        }
    }

    @PostConstruct
    public void load_add_airplane() {
        session = helper.getSessionFactory().openSession();
        session.beginTransaction();
        planes = null;
        //Query q=session.createSQLQuery("SELECT * from airplane_models");
        //Query q = session.createSQLQuery("SELECT A.id, A.type, A.license, A.capacity, A.manufacturer_id FROM airplane_models A, airplanes B WHERE A.id=B.id AND B.owner_company_id IS NOT NULL");
        Query qP = session.createQuery("select plane from Airplane_model model, Airplane plane\n"
                + "where model.id=plane.model_id and plane.owner_company_id is null order by plane.id");
        planes = (List<Airplane>) qP.list();
        Query qM = session.createQuery("select model from Airplane_model model, Airplane plane\n"
                + "where model.id=plane.model_id and plane.owner_company_id is null order by plane.id");
        models = (List<Airplane_model>) qM.list();

    }

    public List<Airplane_model> getModels() {
        return models;
    }

    public void setModels(List<Airplane_model> models) {
        this.models = models;
    }

    public List<Airplane> getPlanes() {
        return planes;
    }

    public void setPlanes(List<Airplane> planes) {
        this.planes = planes;
    }

    public String addPlane(Airplane_model novi) {
        int i = models.indexOf(novi);
        Airplane a = planes.get(i);
        a.setOwner_company_id(user.getCompany());
        session.update(a);
        session.getTransaction().commit();
        return "worker";
    }

}
