/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package beans;

/**
 *
 * @author Vladimir
 */
public class Airplane {

    int id;
    int model_id;
    Integer owner_company_id;

    public int getId() {
        return id;
    }

    public void setId(int id) {
        this.id = id;
    }

    public int getModel_id() {
        return model_id;
    }

    public void setModel_id(int model_id) {
        this.model_id = model_id;
    }

    public Integer getOwner_company_id() {
        return owner_company_id;
    }

    public void setOwner_company_id(Integer owner_company_id) {
        this.owner_company_id = owner_company_id;
    }
    
    
    
    public Airplane() {
    }
    
    
    
}
