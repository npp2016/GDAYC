#ifndef GDAY
#define GDAY

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>



#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

#define STRING_LENGTH 2000

#define FIXED 0
#define GRASSES 1
#define ALLOMETRIC 2

#define MATE 0
#define MEDLYN 0

#define DAILY 0
#define YEARLY 1

#define C3 0
#define C4 1

typedef struct {
    FILE *ifp;  
    char  cfg_fname[STRING_LENGTH];
    char  met_fname[STRING_LENGTH];
    char  out_fname[STRING_LENGTH];
    char  out_param_fname[STRING_LENGTH]; 
    char  git_hash[STRING_LENGTH]; 
    int   adjust_rtslow;     
    int   alloc_model; 
    int   assim_model; 
    int   calc_sw_params;
    int   deciduous_model;
    int   disturbance;
    int   exudation;
    int   fixed_stem_nc;
    int   fixleafnc;
    int   grazing;
    int   gs_model;   
    int   hurricane;   
    int   model_optroot;
    int   modeljm;
    int   ncycle;
    int   num_days;
    int   num_years;
    int   nuptake_model;
    int   output_ascii;        
    int   passiveconst;
    int   print_options; 
    int   ps_pathway; 
    int   respiration_model; 
    int   strfloat; 
    int   sw_stress_model;    
    int   trans_model;
    int   use_eff_nc;  
    int   water_stress;       
    int  *uniq_years;
    int   ndays;  
} control;











typedef struct {
    double activesoil;
    double activesoiln;
    double age;
    double avg_albranch;
    double avg_alcroot;
    double avg_alleaf;
    double avg_alroot;
    double avg_alstem;
    double branch;
    double branchn;
    double canht;
    double croot;
    double crootn;
    double cstore;
    double inorgn;
    double lai;
    double max_lai;
    double max_shoot;
    double metabsoil;
    double metabsoiln;
    double metabsurf;
    double metabsurfn;
    double nstore;
    double passivesoil;
    double passivesoiln;
    double pawater_root;
    double pawater_topsoil;
    double prev_sma;
    double root;
    double root_depth;
    double rootn;
    double sapwood;
    double shoot;
    double shootn;
    double sla;
    double slowsoil;
    double slowsoiln;
    double stem;
    double stemn;
    double stemnimm;
    double stemnmob;
    double structsoil;
    double structsoiln;
    double structsurf;
    double structsurfn;
} state;

typedef struct {
    double a1;
    double actnc0;
    double actncmax;
    double ageold;
    double ageyoung;
    double albedo;
    double alpha;
    double alpha_j;
    double bdecay;
    double brabove;
    double bretrans;
    double callocb;
    double callocbz;
    double callocf;
    double callocfz;
    double callocr;
    double callocrx;
    double callocrz;
    double canht;
    double cfracts;
    double ci_ca_ratio;
    double co2_effect_on_wue;
    double co2_unlim_c_prod;
    double cue;
    double d0;
    double d1;
    double delsj;
    double direct_frac;
    double displace_ratio;
    double dz0v_dh;
    double eaj;
    double eav;
    double edj;
    double epsilon;
    double extraction;
    double faecescn;
    double faecesn;
    double fdecay;
    double fdecaydry;
    double fhw;
    double finesoil;
    double fmleaf;
    double fmroot;
    double fracfaeces;
    double fracteaten;
    double fractosoil;
    double fractup_soil;
    double fretrans;
    double fwpmax_root;
    double fwpmax_tsoil;
    double fwpmin_root;
    double fwpmin_tsoil;
    double g1;
    double growth_efficiency;
    double jmax;
    double jmaxna;
    double jmaxnb;
    double kdec1;
    double kdec2;
    double kdec3;
    double kdec4;
    double kdec5;
    double kdec6;
    double kdec7;
    double kext;
    double kq10;
    double kr;
    double lai_cover;
    double latitude;
    double ligfaeces;
    double ligroot;
    double ligshoot;
    double liteffnc;
    double metfrac0;
    double metfrac1;
    double n_crit;
    double ncbnew;
    double ncbnewz;
    double ncfmin;
    double ncmaxfold;
    double ncmaxfyoung;
    double ncmaxr;
    double ncpower;
    double ncrfac;
    double ncwimm;
    double ncwimmz;
    double ncwnew;
    double ncwnewz;
    double nmin;
    double nmin0;
    double nmincrit;
    double nuptakez;
    double passivesoilnz;
    double passivesoilz;
    double passnc0;
    double passncmax;
    double previous_ncd;
    double rateloss;
    double rateuptake;
    double rdecay;
    double rdecaydry;
    double retransmob;
    double rfmult;
    double rhizresp;
    char   rootsoil_type[STRING_LENGTH];
    double rretrans;
    double slainit;
    double slamax;
    double slazero;
    double slownc0;
    double slowncmax;
    double store_transfer_len;
    double structcn;
    double structrat;
    double theta;
    char   topsoil_type[STRING_LENGTH];
    double uo;
    double vcmax;
    double vcmaxna;
    double vcmaxnb;
    double vxfix;
    double watdecaydry;
    double watdecaywet;
    double wcapac_root;
    double wcapac_topsoil;
    double wdecay;
    double wetloss;
    double wretrans;
    double wue0;
    double z0h_z0m;

} params;

typedef struct {
    int    *year; 
    int    *prjday; 
    double *sw_rad;
    double *tair;
    double *rain;
    double *tsoil;
    double *tam;
    double *tpm;
    double *vpd_am;
    double *vpd_pm;
    double *vpd_avg;
    double *co2;
    double *ndep;
    double *wind; 
    double *atmos_press;
    double *par;
    double *wind_am;
    double *wind_pm;
    double *sw_rad_am;
    double *sw_rad_pm; 
} met;

typedef struct {
    double gpp_gCm2 ;
    double npp_gCm2 ;
    double gpp ;
    double npp ;
    double nep ;
    double auto_resp ;
    double hetero_resp ;
    double retrans ;
    
    /* n */
    double nuptake ;
    double nloss ;
    double npassive ; /* n passive -> active */
    double ngross ;   /* N gross mineralisation */
    double nimmob ;   /* N immobilisation in SOM */
    double nlittrelease ; /* N rel litter = struct + metab */
    double activelossf ; /* frac of active C -> CO2 */
    
    /* water fluxes */
    double wue ;
    double et ;
    double soil_evap ;
    double transpiration ;
    double erain ;
    double interception ;
    double runoff ;
    
    /* daily C production */
    double cpleaf ;
    double cproot ;
    double cpbranch ;
    double cpstem ;
    double cprootexudate ;
    
    /* daily N production */
    double npleaf ;
    double nproot ;
    double npbranch ;
    double npstemimm ;
    double npstemmob ;
    double nrootexudate ;
    
    /* dying stuff */
    double deadleaves ;   /* Leaf litter C production (t/ha/yr) */
    double deadroots ;    /* Root litter C production (t/ha/yr) */
    double deadbranch ;   /* Branch litter C production (t/ha/yr) */
    double deadstems ;    /* Stem litter C production (t/ha/yr) */
    double deadleafn ;    /* Leaf litter N production (t/ha/yr) */
    double deadrootn ;    /* Root litter N production (t/ha/yr) */
    double deadbranchn ;  /* Branch litter N production (t/ha/yr) */
    double deadstemn ;    /* Stem litter N production (t/ha/yr) */
     
    /* grazing stuff */
    double ceaten ;       /* C consumed by grazers (t C/ha/y) */
    double neaten ;       /* N consumed by grazers (t C/ha/y) */
    double faecesc ;      /* Flux determined by faeces C:N */
    double nurine ;       /* Rate of N input to soil in urine (t/ha/y) */
    
    /*C N root/shoot to struct and metab pools */
    double cresid[4]; /* Cshoot -> surf struct, Croot -> soil sturct, Cshoot -> surf metab, Croot ->surf */
    double nresid[4]; /* Nshoot -> surf struct, Nroot -> soil sturct, Nshoot -> surf metab, Nroot ->surf metab */
    double cstruct[4]; /* Csurf struct -> slow, Csurf struct -> active, Csoil struct -> slow, Csoil struct -> active */
    double nstruct[4]; /* Nsurf struct -> slow, Nsurf struct -> active, Nsoil struct -> slow, Nsoil struct -> active */
   
    /* C flows to the air */
    double co2_to_air[7];
  
    double passive;
    
    double nslow[2];  /* Nslow -> active and -> passive */
    double cslow[2];   /* Cslow -> active and -> passive */
    double nmetab[2];  /* N surf metab and N soil metab -> active */
    double cmetab[2];  /* C surf metab and C soil metab -> active */
    double cactive[2];  /* C active -> slow/passive */
    double nactive[2];  /* N active -> slow/passive */
   
    
    double cica_avg ; /* used in water balance, only when running mate model */
    double nmineralisation ;
    double apar ;
    
    double rabove ;
    double microbial_resp ;
    
    double gs_mol_m2_sec ;
    double ga_mol_m2_sec ;
    double omega ;



} fluxes;



void    initialise_control(control *);
void    initialise_fluxes(fluxes *);
void    initialise_state(state *f);
void    prog_error(const char *, const unsigned int);
void    read_met_data(char **, control *, met *);
int     calc_days_in_year(int);
int     parse_ini_file(control *, params *, state *);
int     handler(char *, char *, char *, control *, params *, state *);
char   *rstrip(char *);
char   *lskip(char *);
char   *find_char_or_comment(char*, char);
char   *strncpy0(char*, char*, size_t);
char   *strip_first_and_last_character(char);

/* memory stuff */
void   *allocate_struct(size_t, const char *, const unsigned int);
char   *allocate_memory_char(int, const unsigned int);
short  *allocate_memory_short(int, const unsigned int);
long   *allocate_memory_long(int, const unsigned int);
int    *allocate_memory_int(int, const unsigned int);
float  *allocate_memory_float(int, const unsigned int);
double *allocate_memory_double(int, const unsigned int);


#endif /* GDAY */
