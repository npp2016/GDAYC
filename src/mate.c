/* Model Any Terrestrial Ecosystem (MATE) model. 

Simulates photosyntehsis (GPP) based on Farquahar + von Caemmerer, using the
canopy estimate of LUE derived from the methods of Sands. MATE is connected
to G'DAY via LAI and leaf N content. Key feedback through soil N
mineralisation and plant N uptake. Plant respiration is calculated via
carbon-use efficiency (CUE=NPP/GPP). There is a further water limitation
constraint on productivity through the ci:ca ratio.

References:
-----------
* Medlyn, B. E. et al (2011) Global Change Biology, 17, 2134-2144.
* McMurtrie, R. E. et al. (2008) Functional Change Biology, 35, 521-34.
* Sands, P. J. (1995) Australian Journal of Plant Physiology, 22, 601-14.

Rubisco kinetic parameter values are from:
* Bernacchi et al. (2001) PCE, 24, 253-259.
* Medlyn et al. (2002) PCE, 25, 1167-1179, see pg. 1170.

Photosynthesis is calculated assuming GPP is proportional to APAR,
a commonly assumed reln (e.g. Potter 1993, Myneni 2002). The slope of
relationship btw GPP and APAR, i.e. LUE is modelled using the
photosynthesis eqns from Sands.

Assumptions:
------------
(1) photosynthetic light response is a non-rectangular hyperbolic func
    of photon-flux density with a light-saturatred photosynthetic rate
    (Amax), quantum yield (alpha) and curvature (theta).
(2) the canopy is horizontally uniform.
(3) PAR distribution within the canopy obeys Beer's law.
(4) light-saturated photosynthetic rate declines with canopy depth in
    proportion to decline in PAR
(5) alpha + theta do not vary within the canopy
(6) dirunal variation of PAR is sinusoidal.
(7) The model makes no assumption about N within the canopy, however
    this version assumes N declines exponentially through the cnaopy.


*/

void mate(control_struct *c, params_struct *p, metdata_struct *m, doy, daylen){
    
    double gamstar25 = 42.75;    /* co2 compensation partial pressure in the 
                                   absence of dark resp at 25 
                                   degC [umol mol-1] */
    double Oi = 205000.0;        /* intercellular concentration of O2 */
    double Kc25 = 404.9;         /* Michaelis-Menten coefficents for 
                                   carboxylation by Rubisco at 
                                   25degC [umol mol-1] */
    double Ko25 = 278400.0;      /* Michaelis-Menten coefficents for oxygenation 
                                   by Rubisco at 25degC [umol mol-1]. Note 
                                   value in Bernacchie 2001 is in mmol!! */
    double Ec = 79430.0;         /* Activation energy for carboxylation 
                                   [J mol-1] */
    double Eo = 36380.0;         /* Activation energy for oxygenation [J mol-1]*/
    double Egamma = 37830.0;     /* Activation energy at CO2 compensation point 
                                   [J mol-1] */
    
    double gamma[2], km[2], jmax[2], vcmax[2], alpha, ci[2];
    int i = 0;
    
    
    
    for (i=0; i<2; i++) {
        /* calculate mate parameters, e.g. accounting for temp dependancy */
        gamma_star[i] = calculate_co2_compensation_point(m->tam[doy]);
        km[i] = calculate_michaelis_menten_parameter(m->tam[doy])
        jmax[i] = calculate_jmax_parameter(m->tam[doy], N0)
        vcmax[i] = calculate_vcmax_parameter(m->tpm[doy], N0)
        
        /* calculate ratio of intercellular to atmospheric CO2 concentration.
        Also allows productivity to be water limited through stomatal opening.*/
        ci[i] = calculate_ci_ca_ratio(m->vpd[doy]) * ca;
        
        /* Rubisco-limited rate of photosynthesis */
        ac[i] = aclim(ci[i], gamma_star[i], km[i], vcmax[i]);
        
        /* Light-limited rate of photosynthesis allowed by RuBP regeneration */
        aj[i] = ajlim(jmax[i], ci[i], gamma_star[i]);
        
        /* Note that these are gross photosynthetic rates. */
        asat[i] = min(aj[i], ac[i]);
        
    gamma_star_avg = (gamma_star[c->am] + gamma_star[c->pm]) / 2.0;
    
    /* Quantum yield of photosynthesis from McMurtrie 2008 */
    alpha = 0.07 * ((ca -  gamma_star_avg) / (ca + 2.0 *  gamma_star_avg));
   
    /* store value as needed in water balance calculation */
    f->cica_avg = (ci[c->am] + ci[c->pm]) / 2.0;
    
    
    for (i=0; i<2; i++) {
        /* GPP is assumed to be proportional to APAR, where the LUE defines the
        slope of this relationship. LUE, calculation is performed for morning 
        and afternnon periods */
        lue[i] = epsilon(asat[i], m->par[doy], daylen, alpha);
    
    /* mol C mol-1 PAR - use average to simulate canopy photosynthesis */
    lue_avg = (lue[c->am] + lue[c->pm]) / 2.0;

    if (lai <= 0.000001){
        f->apar = 0.0;
    } else {
        par_mol = par * UMOL_TO_MOL;
        f->apar = par_mol * s->light_interception;
    }
    /* gC m-2 d-1 */
    f->gpp_gCm2 = f->apar * lue_avg * MOLE_C_TO_GRAMS_C;
    f->npp_gCm2 = f->gpp_gCm2 * p->cue;
    
    /* tonnes hectare-1 day-1 */
    conv = G_AS_TONNES / M2_AS_HA;
    f->gpp = f->gpp_gCm2 * conv;
    f->npp = f->npp_gCm2 * conv;
    
    /* Plant respiration assuming carbon-use efficiency. */
    f->auto_resp = f->gpp - f->npp;
    
    return;
}