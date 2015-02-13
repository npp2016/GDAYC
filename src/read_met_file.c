#include "gday.h"

void read_met_data(char **argv, control *c, met *m)
{
    FILE *infile;
    char line[STRING_LENGTH];
    int file_len = 0;
    int i = 0;
    int nvars = 20;
    int unq_yr_counter = 0;
    int skipped_lines = 0;
    printf("%s\n", c->met_fname);
    infile = fopen(c->met_fname, "r");
    if(infile == NULL)
        prog_error("Met file not found", __LINE__);
   
    /* work out how big the file is */
    while (fgets(line, STRING_LENGTH, infile) != NULL) {
        /* ignore comment line */
        if (*line == '#') 
            continue; 
        file_len++;
    }
    rewind(infile);
    c->num_days = file_len-1;
  
    /* allocate memory for meteorological arrays */
    m->year = allocate_memory_int(file_len, __LINE__);
    m->prjday = allocate_memory_int(file_len, __LINE__);
    m->sw_rad = allocate_memory_double(file_len, __LINE__);
    m->tair = allocate_memory_double(file_len, __LINE__);
    m->rain = allocate_memory_double(file_len, __LINE__);
    m->tsoil = allocate_memory_double(file_len, __LINE__);
    m->tam = allocate_memory_double(file_len, __LINE__);
    m->tpm = allocate_memory_double(file_len, __LINE__);
    m->vpd_am = allocate_memory_double(file_len, __LINE__);
    m->vpd_pm = allocate_memory_double(file_len, __LINE__);
    m->vpd_avg = allocate_memory_double(file_len, __LINE__);
    m->co2 = allocate_memory_double(file_len, __LINE__);
    m->ndep = allocate_memory_double(file_len, __LINE__);
    m->wind = allocate_memory_double(file_len, __LINE__);
    m->atmos_press = allocate_memory_double(file_len, __LINE__);
    m->par = allocate_memory_double(file_len, __LINE__);
    m->wind_am = allocate_memory_double(file_len, __LINE__);
    m->wind_pm = allocate_memory_double(file_len, __LINE__);
    m->sw_rad_am = allocate_memory_double(file_len, __LINE__);
    m->sw_rad_pm = allocate_memory_double(file_len, __LINE__);
    
    i = 0;
    skipped_lines = 0;
    while (fgets(line, STRING_LENGTH, infile) != NULL) {
        /* ignore comment line */
        if (*line == '#') { 
            skipped_lines++;
            continue; 
        }
        if (sscanf(line, "%d,%d,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,\
                          %lf,%lf,%lf,%lf,%lf,%lf,%lf", \
                          &(m->year[i]), &(m->prjday[i]), &(m->sw_rad[i]), \
                          &(m->tair[i]), &(m->rain[i]), &(m->tsoil[i]), \
                          &(m->tam[i]), &(m->tpm[i]), &(m->vpd_am[i]), \
                          &(m->vpd_pm[i]), &(m->vpd_avg[i]), &(m->co2[i]), \
                          &(m->ndep[i]), &(m->wind[i]), &(m->atmos_press[i]), \
                          &(m->par[i]), &(m->wind_am[i]), &(m->wind_pm[i]), \
                          &(m->sw_rad_am[i]), &(m->sw_rad_pm[i])) != nvars) {
            fprintf(stderr, "%s: badly formatted input in file on line %d\n", \
                    *argv, (int)i+1+skipped_lines);
            exit(EXIT_FAILURE);
        }
        /* Build an array of the unique years as we loop over the input file */
        if (unq_yr_counter == 0) {
            unq_yr_counter++;
            c->uniq_years = (int*)realloc(c->uniq_years, \
                                          unq_yr_counter * sizeof(int));
            c->uniq_years[unq_yr_counter-1] = m->year[i];
        } else if (c->uniq_years[unq_yr_counter-1] != m->year[i]) {
            unq_yr_counter++;
            c->uniq_years = (int*)realloc(c->uniq_years, \
                                          unq_yr_counter * sizeof(int));
            c->uniq_years[unq_yr_counter-1] = m->year[i];
        }
        
        i++;
    }
    c->num_years = unq_yr_counter-1;
    
    
    fclose(infile);
    return;
}