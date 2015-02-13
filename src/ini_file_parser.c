/* Read the .ini file into the various structures. */

#include "gday.h"

/* 
Loop through the file which is passed on the standard in, and break 
the file up into the relevant sections...
*/
int parse_ini_file(control *c, params *p, state *s) 
{
    
    char line[STRING_LENGTH];
    char section[STRING_LENGTH] = "";
    char prev_name[STRING_LENGTH] = "";
    char *start;
    char *end;
    char *name;
    char *value;
    
    int error = 0;
    int line_number = 0;
    
    while (fgets(line, sizeof(line), c->ifp) != NULL) {
        line_number++;
        start = lskip(rstrip(line));
        if (*start == ';' || *start == '#') {
            /* Per Python ConfigParser, allow '#' comments at start of line */
        }
        else if (*start == '[') {
            /* A "[section]" line */
            end = find_char_or_comment(start + 1, ']');
            if (*end == ']') {
                *end = '\0';
                strncpy0(section, start + 1, sizeof(section));
                *prev_name = '\0';
            }
            else if (!error) {
                /* No ']' found on section line */
                error = line_number;
                
            }
        }
        else if (*start && *start != ';') {
            /* Not a comment, must be a name[=:]value pair */
            end = find_char_or_comment(start, '=');
            if (*end != '=') {
                end = find_char_or_comment(start, ':');
            }
            if (*end == '=' || *end == ':') {
                *end = '\0';
                name = rstrip(start);
                value = lskip(end + 1);
                end = find_char_or_comment(value, '\0');
                if (*end == ';')
                    *end = '\0';
                rstrip(value);

                /* Valid name[=:]value pair found, call handler */
                strncpy0(prev_name, name, sizeof(prev_name));
                
                if (!handler(section, name, value, c, p, s) && !error) 
                    error = line_number;
            }
            else if (!error) {
                /* No '=' or ':' found on name[=:]value line */
                error = line_number;
                break;
            }
        }
    }
    return error;
}

/* Strip whitespace chars off end of given string, in place. Return s. */
char* rstrip(char* s)
{
    char* p = s + strlen(s);
    while (p > s && isspace(*--p)) *p = '\0';
    return s;
}

/* Return pointer to first non-whitespace char in given string. */
char* lskip(char* s)
{
    while (*s && isspace(*s)) s++;
    return (char*)s;
}


/*
Return pointer to first char c or ';' comment in given string, or 
pointer to null at end of string if neither found. ';' must be 
prefixed by a whitespace character to register as a comment. 
*/
char* find_char_or_comment(char* s, char c)
{
    
    int was_whitespace = 0;
    while (*s && *s != c && !(was_whitespace && *s == ';')) {
        was_whitespace = isspace(*s);
        s++;
    }
    return (char*)s;
}

/* Version of strncpy that ensures dest (size bytes) is null-terminated. */
char *strncpy0(char* dest, char* src, size_t size)
{
    strncpy(dest, src, size);
    dest[size - 1] = '\0';
    return dest;
}

/* Assigns the values from the .INI file straight into the various structures */
int handler(char *section, char *name, char *value, control *c, 
            params *p, state *s) 
{
    char *temp = value;
    
    #define MATCH(s, n) strcasecmp(section, s) == 0 && strcasecmp(name, n) == 0
    
    /**
     ** Control 
     **/
     
    /* GIT */
    if (MATCH("git", "git_hash")) {
        strcpy(c->git_hash, temp);
    } 
    
    /* FILES */
    if (MATCH("files", "cfg_fname")) {
        /* remove quotation marks around the string */
	    temp++; /* removes first quote */
	    temp[strlen(temp)-1] = 0; /* removes last quote */
        strcpy(c->cfg_fname, temp);
    } else if (MATCH("files", "met_fname")) {
        
        strcpy(c->met_fname, temp);
    } else if (MATCH("files", "out_fname")) {
        /* remove quotation marks around the string */
	    temp++; /* removes first quote */
	    temp[strlen(temp)-1] = 0; /* removes last quote */
        strcpy(c->out_fname, temp);
    } else if (MATCH("files", "out_param_fname")) {
        /* remove quotation marks around the string */
	    temp++; /* removes first quote */
	    temp[strlen(temp)-1] = 0; /* removes last quote */
        strcpy(c->out_param_fname, temp);   
    }
    
    /* CONTROL */
    if (MATCH("control", "adjust_rtslow")) {
        c->adjust_rtslow = atoi(value);
    } else if (MATCH("control", "alloc_model")) {
        c->alloc_model = atoi(value);
    } else if (MATCH("control", "assim_model")) {
        c->assim_model = atoi(value);
    } else if (MATCH("control", "calc_sw_params")) {
        c->calc_sw_params = atoi(value);
    } else if (MATCH("control", "deciduous_model")) {
        c->deciduous_model = atoi(value);
    } else if (MATCH("control", "disturbance")) {
        c->disturbance = atoi(value);
    } else if (MATCH("control", "exudation")) {
        c->exudation = atoi(value);
    } else if (MATCH("control", "fixed_stem_nc")) {
        c->fixed_stem_nc = atoi(value);
    } else if (MATCH("control", "fixleafnc")) {
        c->fixleafnc = atoi(value);
    } else if (MATCH("control", "grazing")) {
        c->grazing = atoi(value);
    } else if (MATCH("control", "gs_model")) {
        c->gs_model = atoi(value);
    } else if (MATCH("control", "hurricane")) {
        c->hurricane = atoi(value);  
    } else if (MATCH("control", "model_optroot")) {
        c->model_optroot = atoi(value);
    } else if (MATCH("control", "modeljm")) {
        c->modeljm = atoi(value);
    } else if (MATCH("control", "ncycle")) {
        c->ncycle = atoi(value);
    } else if (MATCH("control", "nuptake_model")) {
        c->nuptake_model = atoi(value);
    } else if (MATCH("control", "output_ascii")) {
        c->output_ascii = atoi(value);
    } else if (MATCH("control", "passiveconst")) {
        c->passiveconst = atoi(value);
    } else if (MATCH("control", "print_options")) {
        c->print_options = atoi(value);
    } else if (MATCH("control", "ps_pathway")) {
        c->ps_pathway = atoi(value); 
     } else if (MATCH("control", "respiration_model")) {
        c->respiration_model = atoi(value); 
    } else if (MATCH("control", "strfloat")) {
        c->strfloat = atoi(value);
    } else if (MATCH("control", "sw_stress_model")) {
        c->sw_stress_model = atoi(value);
    } else if (MATCH("control", "trans_model")) {
        c->trans_model = atoi(value);
    } else if (MATCH("control", "use_eff_nc")) {
        c->use_eff_nc = atoi(value);
    } else if (MATCH("control", "water_stress")) {
        c->water_stress = atoi(value);
    }
    
 
 
    /**
     ** State
     **/
    if (MATCH("state", "activesoil")) {
        s->activesoil = atof(value);
    } else if (MATCH("state", "activesoiln")) {
        s->activesoiln = atof(value);
    } else if (MATCH("state", "age")) {
        s->age = atof(value);
    } else if (MATCH("state", "avg_albranch")) {
        s->avg_albranch = atof(value);
    } else if (MATCH("state", "avg_alcroot")) {
        s->avg_alcroot = atof(value);
    } else if (MATCH("state", "avg_alleaf")) {
        s->avg_alleaf = atof(value);
    } else if (MATCH("state", "avg_alroot")) {
        s->avg_alroot = atof(value);
    } else if (MATCH("state", "avg_alstem")) {
        s->avg_alstem = atof(value);
    } else if (MATCH("state", "branch")) {
        s->branch = atof(value);
    } else if (MATCH("state", "branchn")) {
        s->branchn = atof(value);
    } else if (MATCH("state", "canht")) {
        s->canht = atof(value);
    } else if (MATCH("state", "croot")) {
        s->croot = atof(value);
    } else if (MATCH("state", "crootn")) {
        s->crootn = atof(value);
    } else if (MATCH("state", "cstore")) {
        s->cstore = atof(value);
    } else if (MATCH("state", "inorgn")) {
        s->inorgn = atof(value);
    } else if (MATCH("state", "lai")) {
        s->lai = atof(value);
    } else if (MATCH("state", "max_lai")) {
        s->max_lai = atof(value);
    } else if (MATCH("state", "max_shoot")) {
        s->max_shoot = atof(value);
    } else if (MATCH("state", "metabsoil")) {
        s->metabsoil = atof(value);
    } else if (MATCH("state", "metabsoiln")) {
        s->metabsoiln = atof(value);
    } else if (MATCH("state", "metabsurf")) {
        s->metabsurf = atof(value);
    } else if (MATCH("state", "metabsurfn")) {
        s->metabsurfn = atof(value);
    } else if (MATCH("state", "nstore")) {
        s->nstore = atof(value);
    } else if (MATCH("state", "passivesoil")) {
        s->passivesoil = atof(value);
    } else if (MATCH("state", "passivesoiln")) {
        s->passivesoiln = atof(value);
    } else if (MATCH("state", "pawater_root")) {
        s->pawater_root = atof(value);
    } else if (MATCH("state", "pawater_topsoil")) {
        s->pawater_topsoil = atof(value);
    } else if (MATCH("state", "prev_sma")) {
        s->prev_sma = atof(value);
    } else if (MATCH("state", "root")) {
        s->root = atof(value);
    } else if (MATCH("state", "root_depth")) {
        s->root_depth = atof(value);
    } else if (MATCH("state", "rootn")) {
        s->rootn = atof(value);
    } else if (MATCH("state", "sapwood")) {
        s->sapwood = atof(value);
    } else if (MATCH("state", "shoot")) {
        s->shoot = atof(value);
    } else if (MATCH("state", "shootn")) {
        s->shootn = atof(value);
    } else if (MATCH("state", "sla")) {
        s->sla = atof(value);
    } else if (MATCH("state", "slowsoil")) {
        s->slowsoil = atof(value);
    } else if (MATCH("state", "slowsoiln")) {
        s->slowsoiln = atof(value);
    } else if (MATCH("state", "stem")) {
        s->stem = atof(value);
    } else if (MATCH("state", "stemn")) {
        s->stemn = atof(value);
    } else if (MATCH("state", "stemnimm")) {
        s->stemnimm = atof(value);
    } else if (MATCH("state", "stemnmob")) {
        s->stemnmob = atof(value);
    } else if (MATCH("state", "structsoil")) {
        s->structsoil = atof(value);
    } else if (MATCH("state", "structsoiln")) {
        s->structsoiln = atof(value);
    } else if (MATCH("state", "structsurf")) {
        s->structsurf = atof(value);
    } else if (MATCH("state", "structsurfn")) {
        s->structsurfn = atof(value);
    }
    
    /* Params */
    if (MATCH("params", "a1")) {
        p->a1 = atof(value);
    } else if (MATCH("params", "actnc0")) {
        p->actnc0 = atof(value);
    } else if (MATCH("params", "actncmax")) {
        p->actncmax = atof(value);
    } else if (MATCH("params", "ageold")) {
        p->ageold = atof(value);
    } else if (MATCH("params", "ageyoung")) {
        p->ageyoung = atof(value);
    } else if (MATCH("params", "albedo")) {
        p->albedo = atof(value);
    } else if (MATCH("params", "alpha")) {
        p->alpha = atof(value);
    } else if (MATCH("params", "alpha_j")) {
        p->alpha_j = atof(value);
    } else if (MATCH("params", "bdecay")) {
        p->bdecay = atof(value);
    } else if (MATCH("params", "brabove")) {
        p->brabove = atof(value);
    } else if (MATCH("params", "bretrans")) {
        p->bretrans = atof(value);
    } else if (MATCH("params", "callocb")) {
        p->callocb = atof(value);
    } else if (MATCH("params", "callocbz")) {
        p->callocbz = atof(value);
    } else if (MATCH("params", "callocf")) {
        p->callocf = atof(value);
    } else if (MATCH("params", "callocfz")) {
        p->callocfz = atof(value);
    } else if (MATCH("params", "callocr")) {
        p->callocr = atof(value);
    } else if (MATCH("params", "callocrx")) {
        p->callocrx = atof(value);
    } else if (MATCH("params", "callocrz")) {
        p->callocrz = atof(value);
    } else if (MATCH("params", "canht")) {
        p->canht = atof(value);
    } else if (MATCH("params", "cfracts")) {
        p->cfracts = atof(value);
    } else if (MATCH("params", "ci_ca_ratio")) {
        p->ci_ca_ratio = atof(value);
    } else if (MATCH("params", "co2_effect_on_wue")) {
        p->co2_effect_on_wue = atof(value);
    } else if (MATCH("params", "co2_unlim_c_prod")) {
        p->co2_unlim_c_prod = atof(value);
    } else if (MATCH("params", "cue")) {
        p->cue = atof(value);
    } else if (MATCH("params", "d0")) {
        p->d0 = atof(value);
    } else if (MATCH("params", "d1")) {
        p->d1 = atof(value);
    } else if (MATCH("params", "delsj")) {
        p->delsj = atof(value);
    } else if (MATCH("params", "direct_frac")) {
        p->direct_frac = atof(value);
    } else if (MATCH("params", "displace_ratio")) {
        p->displace_ratio = atof(value);
    } else if (MATCH("params", "dz0v_dh")) {
        p->dz0v_dh = atof(value);
    } else if (MATCH("params", "eaj")) {
        p->eaj = atof(value);
    } else if (MATCH("params", "eav")) {
        p->eav = atof(value);
    } else if (MATCH("params", "edj")) {
        p->edj = atof(value);
    } else if (MATCH("params", "epsilon")) {
        p->epsilon = atof(value);
    } else if (MATCH("params", "extraction")) {
        p->extraction = atof(value);
    } else if (MATCH("params", "faecescn")) {
        p->faecescn = atof(value);
    } else if (MATCH("params", "faecesn")) {
        p->faecesn = atof(value);
    } else if (MATCH("params", "fdecay")) {
        p->fdecay = atof(value);
    } else if (MATCH("params", "fdecaydry")) {
        p->fdecaydry = atof(value);
    } else if (MATCH("params", "fhw")) {
        p->fhw = atof(value);
    } else if (MATCH("params", "finesoil")) {
        p->finesoil = atof(value);
    } else if (MATCH("params", "fmleaf")) {
        p->fmleaf = atof(value);
    } else if (MATCH("params", "fmroot")) {
        p->fmroot = atof(value);
    } else if (MATCH("params", "fracfaeces")) {
        p->fracfaeces = atof(value);
    } else if (MATCH("params", "fracteaten")) {
        p->fracteaten = atof(value);
    } else if (MATCH("params", "fractosoil")) {
        p->fractosoil = atof(value);
    } else if (MATCH("params", "fractup_soil")) {
        p->fractup_soil = atof(value);
    } else if (MATCH("params", "fretrans")) {
        p->fretrans = atof(value);
    } else if (MATCH("params", "fwpmax_root")) {
        p->fwpmax_root = atof(value);
    } else if (MATCH("params", "fwpmax_tsoil")) {
        p->fwpmax_tsoil = atof(value);
    } else if (MATCH("params", "fwpmin_root")) {
        p->fwpmin_root = atof(value);
    } else if (MATCH("params", "fwpmin_tsoil")) {
        p->fwpmin_tsoil = atof(value);
    } else if (MATCH("params", "g1")) {
        p->g1 = atof(value);
    } else if (MATCH("params", "growth_efficiency")) {
        p->growth_efficiency = atof(value);
    } else if (MATCH("params", "jmax")) {
        p->jmax = atof(value);
    } else if (MATCH("params", "jmaxna")) {
        p->jmaxna = atof(value);
    } else if (MATCH("params", "jmaxnb")) {
        p->jmaxnb = atof(value);
    } else if (MATCH("params", "kdec1")) {
        p->kdec1 = atof(value);
    } else if (MATCH("params", "kdec2")) {
        p->kdec2 = atof(value);
    } else if (MATCH("params", "kdec3")) {
        p->kdec3 = atof(value);
    } else if (MATCH("params", "kdec4")) {
        p->kdec4 = atof(value);
    } else if (MATCH("params", "kdec5")) {
        p->kdec5 = atof(value);
    } else if (MATCH("params", "kdec6")) {
        p->kdec6 = atof(value);
    } else if (MATCH("params", "kdec7")) {
        p->kdec7 = atof(value);
    } else if (MATCH("params", "kext")) {
        p->kext = atof(value);
    } else if (MATCH("params", "kq10")) {
        p->kq10 = atof(value);
    } else if (MATCH("params", "kr")) {
        p->kr = atof(value);
    } else if (MATCH("params", "lai_cover")) {
        p->lai_cover = atof(value);
    } else if (MATCH("params", "latitude")) {
        p->latitude = atof(value);
    } else if (MATCH("params", "ligfaeces")) {
        p->ligfaeces = atof(value);
    } else if (MATCH("params", "ligroot")) {
        p->ligroot = atof(value);
    } else if (MATCH("params", "ligshoot")) {
        p->ligshoot = atof(value);
    } else if (MATCH("params", "liteffnc")) {
        p->liteffnc = atof(value);
    } else if (MATCH("params", "metfrac0")) {
        p->metfrac0 = atof(value);
    } else if (MATCH("params", "metfrac1")) {
        p->metfrac1 = atof(value);
    } else if (MATCH("params", "n_crit")) {
        p->n_crit = atof(value);
    } else if (MATCH("params", "ncbnew")) {
        p->ncbnew = atof(value);
    } else if (MATCH("params", "ncbnewz")) {
        p->ncbnewz = atof(value);
    } else if (MATCH("params", "ncfmin")) {
        p->ncfmin = atof(value);
    } else if (MATCH("params", "ncmaxfold")) {
        p->ncmaxfold = atof(value);
    } else if (MATCH("params", "ncmaxfyoung")) {
        p->ncmaxfyoung = atof(value);
    } else if (MATCH("params", "ncmaxr")) {
        p->ncmaxr = atof(value);
    } else if (MATCH("params", "ncpower")) {
        p->ncpower = atof(value);
    } else if (MATCH("params", "ncrfac")) {
        p->ncrfac = atof(value);
    } else if (MATCH("params", "ncwimm")) {
        p->ncwimm = atof(value);
    } else if (MATCH("params", "ncwimmz")) {
        p->ncwimmz = atof(value);
    } else if (MATCH("params", "ncwnew")) {
        p->ncwnew = atof(value);
    } else if (MATCH("params", "ncwnewz")) {
        p->ncwnewz = atof(value);
    } else if (MATCH("params", "nmin")) {
        p->nmin = atof(value);
    } else if (MATCH("params", "nmin0")) {
        p->nmin0 = atof(value);
    } else if (MATCH("params", "nmincrit")) {
        p->nmincrit = atof(value);
    } else if (MATCH("params", "nuptakez")) {
        p->nuptakez = atof(value);
    } else if (MATCH("params", "passivesoilnz")) {
        p->passivesoilnz = atof(value);
    } else if (MATCH("params", "passivesoilz")) {
        p->passivesoilz = atof(value);
    } else if (MATCH("params", "passnc0")) {
        p->passnc0 = atof(value);
    } else if (MATCH("params", "passncmax")) {
        p->passncmax = atof(value);
    } else if (MATCH("params", "previous_ncd")) {
        p->previous_ncd = atof(value);
    } else if (MATCH("params", "rateloss")) {
        p->rateloss = atof(value);
    } else if (MATCH("params", "rateuptake")) {
        p->rateuptake = atof(value);
    } else if (MATCH("params", "rdecay")) {
        p->rdecay = atof(value);
    } else if (MATCH("params", "rdecaydry")) {
        p->rdecaydry = atof(value);
    } else if (MATCH("params", "retransmob")) {
        p->retransmob = atof(value);
    } else if (MATCH("params", "rfmult")) {
        p->rfmult = atof(value);
    } else if (MATCH("params", "rhizresp")) {
        p->rhizresp = atof(value);
    } else if (MATCH("params", "rootsoil_type")) {
        strcpy(p->rootsoil_type, value);
    } else if (MATCH("params", "rretrans")) {
        p->rretrans = atof(value);
    } else if (MATCH("params", "slainit")) {
        p->slainit = atof(value);
    } else if (MATCH("params", "slamax")) {
        p->slamax = atof(value);
    } else if (MATCH("params", "slazero")) {
        p->slazero = atof(value);
    } else if (MATCH("params", "slownc0")) {
        p->slownc0 = atof(value);
    } else if (MATCH("params", "slowncmax")) {
        p->slowncmax = atof(value);
    } else if (MATCH("params", "store_transfer_len")) {
        p->store_transfer_len = atof(value);
    } else if (MATCH("params", "structcn")) {
        p->structcn = atof(value);
    } else if (MATCH("params", "structrat")) {
        p->structrat = atof(value);
    } else if (MATCH("params", "theta")) {
        p->theta = atof(value);
    } else if (MATCH("params", "topsoil_type")) {
        strcpy(p->topsoil_type, value);
    } else if (MATCH("params", "uo")) {
        p->uo = atof(value);
    } else if (MATCH("params", "vcmax")) {
        p->vcmax = atof(value);
    } else if (MATCH("params", "vcmaxna")) {
        p->vcmaxna = atof(value);
    } else if (MATCH("params", "vcmaxnb")) {
        p->vcmaxnb = atof(value);
    } else if (MATCH("params", "vxfix")) {
        p->vxfix = atof(value);
    } else if (MATCH("params", "watdecaydry")) {
        p->watdecaydry = atof(value);
    } else if (MATCH("params", "watdecaywet")) {
        p->watdecaywet = atof(value);
    } else if (MATCH("params", "wcapac_root")) {
        p->wcapac_root = atof(value);
    } else if (MATCH("params", "wcapac_topsoil")) {
        p->wcapac_topsoil = atof(value);
    } else if (MATCH("params", "wdecay")) {
        p->wdecay = atof(value);
    } else if (MATCH("params", "wetloss")) {
        p->wetloss = atof(value);
    } else if (MATCH("params", "wretrans")) {
        p->wretrans = atof(value);
    } else if (MATCH("params", "wue0")) {
        p->wue0 = atof(value);
    } else if (MATCH("params", "z0h_z0m")) {
        p->z0h_z0m = atof(value);
    }
        
    return 1;
}



