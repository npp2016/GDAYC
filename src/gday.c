#include "gday.h"


int main(int argc, char **argv) 
{
    int i, j = 0;
    int project_day = 0;
    int error = 0;
    
    /**
    *** Setup structures, initialise stuff, e.g. zero fluxes.
    **/
    control *c;
    met *m;
    params *p;
    state *s;
    fluxes *f;
    c = allocate_struct(sizeof(control), "control", __LINE__);
    m = allocate_struct(sizeof(met), "met", __LINE__);
    p = allocate_struct(sizeof(params), "params", __LINE__);
    s = allocate_struct(sizeof(state), "state", __LINE__);
    f = allocate_struct(sizeof(fluxes), "fluxes", __LINE__);
    initialise_control(c);
    initialise_fluxes(f);
    initialise_state(s);
    
	
    /**
    *** Read .ini parameter file and meterological data
    **/
    if (isatty(0)) 
        prog_error("Can't find input file on stdin, try again...?", __LINE__);
    
    error = parse_ini_file(c, p, s); 
    if (error != 0) {
		prog_error("Error reading .INI file on line", __LINE__);
	}
    read_met_data(argv, c, m);
    
    /**
    *** Run GDAY
    **/
    
    /* begin yearly loop */
    for (i=0; i<c->num_years; i++) {
        c->ndays = calc_days_in_year(c->uniq_years[i]);
        
       /* begin daily loop */
        for (j=0; j<c->ndays; j++) {
            printf("%f\n", m->tair[project_day] );
            
            project_day++;
        }   /* end daily loop */
        
    }   /* end yearly loop */
    printf("%s\n", c->git_hash);
    
    
    
    /* clean up */
    free(c);
    c = NULL;
    free(m);
    m = NULL;
    free(p);
    p = NULL;
    free(s);
    s = NULL;
    
    exit(EXIT_SUCCESS);
}



void initialise_control(control *c) {
    /*
    *** Default values for control structure. 
    */
     
    strcpy(c->git_hash, "Err");
    
    c->ifp = stdin;
    strcpy(c->cfg_fname, "Err");
    strcpy(c->met_fname, "Err");
    strcpy(c->out_fname, "Err");
    strcpy(c->out_param_fname, "Err");
    
    c->adjust_rtslow = FALSE;
    c->alloc_model = ALLOMETRIC;
    c->assim_model = MATE;
    c->calc_sw_params = FALSE;
    c->deciduous_model = FALSE;
    c->disturbance = 0;
    c->exudation = FALSE;
    c->fixed_stem_nc = TRUE;
    c->fixleafnc = FALSE;
    c->grazing = FALSE;
    c->gs_model = MEDLYN;
    c->hurricane = 0;
    c->model_optroot = FALSE;
    c->modeljm = 2;
    c->ncycle = TRUE;
    c->nuptake_model = 2;
    c->output_ascii = TRUE;
    c->passiveconst = FALSE;
    c->print_options = DAILY;
    c->ps_pathway = C3;
    c->respiration_model = FIXED;
    c->strfloat = 0;
    c->sw_stress_model = 1;
    c->trans_model = 1;
    c->use_eff_nc = 0;
    c->water_stress = TRUE;
    
    c->num_days = 0;
    c->num_years = 0;
 
    c->uniq_years = NULL;
    c->ndays = 0;
    
    return;
}


void initialise_fluxes(fluxes *f) {
    /*
    *** Default values for fluxes structure. 
    */
    int i = 0;
    
    f->gpp_gCm2 = 0.0;
    f->npp_gCm2 = 0.0;
    f->gpp = 0.0;
    f->npp = 0.0;
    f->nep = 0.0;
    f->auto_resp = 0.0;
    f->hetero_resp = 0.0;
    f->retrans = 0.0;
    
    /* n */
    f->nuptake = 0.0;
    f->nloss = 0.0;
    f->npassive = 0.0; /* n passive -> active */
    f->ngross = 0.0;   /* N gross mineralisation */
    f->nimmob = 0.0;   /* N immobilisation in SOM */
    f->nlittrelease = 0.0; /* N rel litter = struct + metab */
    f->activelossf = 0.0; /* frac of active C -> CO2 */
    
    /* water fluxes */
    f->wue = 0.0;
    f->et = 0.0;
    f->soil_evap = 0.0;
    f->transpiration = 0.0;
    f->erain = 0.0;
    f->interception = 0.0;
    f->runoff = 0.0;
    
    /* daily C production */
    f->cpleaf = 0.0;
    f->cproot = 0.0;
    f->cpbranch = 0.0;
    f->cpstem = 0.0;
    f->cprootexudate = 0.0;
    
    /* daily N production */
    f->npleaf = 0.0;
    f->nproot = 0.0;
    f->npbranch = 0.0;
    f->npstemimm = 0.0;
    f->npstemmob = 0.0;
    f->nrootexudate = 0.0;
    
    /* dying stuff */
    f->deadleaves = 0.0;   /* Leaf litter C production (t/ha/yr) */
    f->deadroots = 0.0;    /* Root litter C production (t/ha/yr) */
    f->deadbranch = 0.0;   /* Branch litter C production (t/ha/yr) */
    f->deadstems = 0.0;    /* Stem litter C production (t/ha/yr) */
    f->deadleafn = 0.0;    /* Leaf litter N production (t/ha/yr) */
    f->deadrootn = 0.0;    /* Root litter N production (t/ha/yr) */
    f->deadbranchn = 0.0;  /* Branch litter N production (t/ha/yr) */
    f->deadstemn = 0.0;    /* Stem litter N production (t/ha/yr) */
     
    /* grazing stuff */
    f->ceaten = 0.0;       /* C consumed by grazers (t C/ha/y) */
    f->neaten = 0.0;       /* N consumed by grazers (t C/ha/y) */
    f->faecesc = 0.0;      /* Flux determined by faeces C:N */
    f->nurine = 0.0;       /* Rate of N input to soil in urine (t/ha/y) */
    
    /*C N root/shoot to struct and metab pools */
    for (i=0; i<4; i++) {
        f->cresid[i] = 0.0; /* Cshoot -> surf struct, Croot -> soil sturct, Cshoot -> surf metab, Croot ->surf */
        f->nresid[i] = 0.0; /* Nshoot -> surf struct, Nroot -> soil sturct, Nshoot -> surf metab, Nroot ->surf metab */
        f->cstruct[i] = 0.0; /* Csurf struct -> slow, Csurf struct -> active, Csoil struct -> slow, Csoil struct -> active */
        f->nstruct[i] = 0.0; /* Nsurf struct -> slow, Nsurf struct -> active, Nsoil struct -> slow, Nsoil struct -> active */
    }
    
    /* C flows to the air */
    for (i=0; i<7; i++) {
        f->co2_to_air[i] = 0.0; 
    }
    
    f->passive = 0.0;
    
    for (i=0; i<2; i++) {
        f->nslow[i] = 0.0;  /* Nslow -> active and -> passive */
        f->cslow[i] = 0.0;   /* Cslow -> active and -> passive */
        f->nmetab[i] = 0.0;  /* N surf metab and N soil metab -> active */
        f->cmetab[i] = 0.0;  /* C surf metab and C soil metab -> active */
        f->cactive[i] = 0.0;  /* C active -> slow/passive */
        f->nactive[i] = 0.0;  /* N active -> slow/passive */
    }
    
    f->cica_avg = 0.0; /* used in water balance, only when running mate model */
    f->nmineralisation = 0.0;
    f->apar = 0.0;
    
    f->rabove = 0.0;
    f->microbial_resp = 0.0;
    
    f->gs_mol_m2_sec = 0.0;
    f->ga_mol_m2_sec = 0.0;
    f->omega = 0.0;



    return;
}



void initialise_state(state *s) {
    
    /*
    *** Default values for state structure. 
    */
    
    s->activesoil = 2.53010543182;
    s->activesoiln = 0.833516379296;
    s->age = 12.0;
    s->avg_albranch = 0.0;
    s->avg_alcroot = 0.0;
    s->avg_alleaf = 0.0;
    s->avg_alroot = 0.0;
    s->avg_alstem = 0.0;
    s->branch = 14.5137000708;
    s->branchn = 0.0442890661217;
    s->canht = 23.0964973582;
    s->croot = 0.0;
    s->crootn = 0.0;
    s->cstore = 0.01;
    s->inorgn = 0.0274523714275;
    s->max_lai = -999.9;
    s->max_shoot = -999.9;
    s->metabsoil = 0.135656771805;
    s->metabsoiln = 0.00542627087221;
    s->metabsurf = 0.0336324759951;
    s->metabsurfn = 0.0013452990398;
    s->nstore = 0.01;
    s->passivesoil = 59.5304597863;
    s->passivesoiln = 8.0134056319;
    s->pawater_root = 94.0415606424;
    s->pawater_topsoil = 24.7780118747;
    s->prev_sma = 1.0;
    s->root = 3.92887790342;
    s->root_depth = -9999.9;
    s->rootn = 0.076296932914;
    s->sapwood = 51.2600270003;
    s->shoot = 4.37991243755;
    s->shootn = 0.0978837857406;
    s->sla = 4.4;
    s->slowsoil = 46.8769593608;
    s->slowsoiln = 2.90664959452;
    s->stem = 87.6580936643;
    s->stemn = 0.263722246902;
    s->stemnimm = 0.263336697464;
    s->stemnmob = 0.00038554943772;
    s->structsoil = 0.917128200367;
    s->structsoiln = 0.00611418800245;
    s->structsurf = 7.10566198821;
    s->structsurfn = 0.0473710799214;
    
    return;
}

void prog_error(const char *reason, const unsigned int line)
{
    fprintf(stderr, "%s, failed at line: %d\n", reason, line);
	exit(EXIT_FAILURE);

    return;
}

int calc_days_in_year(int year)
{
    int ndays = 365;
    
    if (((year % 4==0) && (year % 100 != 0)) || (year % 400 == 0))
         ndays = 366;
    
    return ndays;


}