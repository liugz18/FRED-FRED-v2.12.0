from ast import In
from sre_constants import IN
import numpy as np
import pandas as pd
import csv
import os
from sqlalchemy import null




def reform_data(fips):
    folder = f'2010_ver1_{fips}'
    os.system(f"mkdir {folder}")
    ############# gq.txt #############
    # gq_old = pd.read_csv('2010_ver1_42003_synth_gq.txt', sep=",") #,names=['ID','visit'])
    gq_new = pd.read_csv(fips + '/' + 'gq.txt', sep="\t") 

    gq_new_done = gq_new[['sp_id', 'gq_type', 'persons', 'stcotrbg', 'latitude', 'longitude']]
    stcotrbgs = list(gq_new_done['stcotrbg'])
    persons_list = list(gq_new_done['persons'])
    gq_new_done['stcotrbg'] = gq_new_done['stcotrbg'].astype(str)
    gq_new_done['stcotrbg'] = [ '\"' + i + '\"' for i in gq_new_done['stcotrbg'] ]
    gq_new_done['gq_type'] = [ '\"' + i + '\"' for i in gq_new_done['gq_type'] ]
    # from IPython import embed; embed()
    gq_new_done.to_csv(f'{folder}/2010_ver1_{fips}_synth_gq.txt', sep=',', index=0, quoting=csv.QUOTE_NONE)




    ############# gq_people.txt #############
    # gq_people_old = pd.read_csv('2010_ver1_42003_synth_gq_people.txt', sep=",") #,names=['ID','visit'])
    gq_people_new = pd.read_csv(fips + '/' + 'gq_people.txt', sep="\t") 
    sporder = [1 for i in range(len(gq_people_new))]
    sexs = list(gq_people_new['sex'])
    sexs_done = [1 if i == 'M' else 2 for i in gq_people_new['sex']]

    gq_people_new['sex'] = sexs_done
    gq_people_new['sporder'] = sporder
    gq_people_new_done = gq_people_new[['sp_id', 'sp_gq_id', 'sporder', 'age', 'sex']]
    gq_people_new_done.to_csv(f'{folder}/2010_ver1_{fips}_synth_gq_people.txt', sep=',', index=0, quoting=csv.QUOTE_NONE)

    ############# workplaces.txt #############
    # gq_people_old = pd.read_csv('2010_ver1_42003_synth_gq_people.txt', sep=",") #,names=['ID','visit'])
    workplaces_new = pd.read_csv(fips + '/' + 'workplaces.txt', sep="\t") 
    workers = [np.random.poisson(20) for i in range(len(workplaces_new))]


    workplaces_new['workers'] = workers

    workplaces_new = workplaces_new[['sp_id', 'workers', 'latitude', 'longitude']]
    workplaces_new.to_csv(f'{folder}/2010_ver1_{fips}_workplaces.txt', sep=',', index=0, quoting=csv.QUOTE_NONE)

    ############# households.txt #############
    # gq_people_old = pd.read_csv('2010_ver1_42003_synth_gq_people.txt', sep=",") #,names=['ID','visit'])
    households_new = pd.read_csv(fips + '/' + 'households.txt', sep="\t") 
    serialno = [2009000388518 for i in range(len(households_new))]
    hh_size = [24 for i in range(len(households_new))]
    hh_age = [np.random.poisson(1) for i in range(len(households_new))]


    households_new['serialno'] = serialno
    households_new['hh_size'] = hh_size
    households_new['hh_age'] = hh_age

    households_new['stcotrbg'] = households_new['stcotrbg'].astype(str)
    households_new['stcotrbg'] = [ '\"' + i + '\"' for i in households_new['stcotrbg'] ]
    households_new = households_new[['sp_id', 'serialno', 'stcotrbg', 'hh_race', 'hh_income', 'hh_size', 'hh_age', 'latitude', 'longitude']]
    households_new.to_csv(f'{folder}/2010_ver1_{fips}_synth_households.txt', sep=',', index=0, quoting=csv.QUOTE_NONE)



    ############# schools.txt #############
    # gq_people_old = pd.read_csv('2010_ver1_42003_synth_gq_people.txt', sep=",") #,names=['ID','visit'])
    schools_new = pd.read_csv(fips + '/' + 'schools.txt', sep="\t") 
    names = ["LOGAN MS" for i in range(len(schools_new))]
    stabbr = ["PA" for i in range(len(schools_new))]
    address = ["6202 ALDER ST" for i in range(len(schools_new))]
    city = ["PITTSBURGH" for i in range(len(schools_new))]
    county = ["ALLEGHENY" for i in range(len(schools_new))]
    zipcode = [0 for i in range(len(schools_new))]
    zip4 = [0 for i in range(len(schools_new))]
    nces_id = ["A0501924" for i in range(len(schools_new))]
    total = [428896 for i in range(len(schools_new))]
    prek = [0 for i in range(len(schools_new))]
    kinder = [428896 for i in range(len(schools_new))]
    gr01_gr12 = [0.0 for i in range(len(schools_new))]
    ungraded = [0.0 for i in range(len(schools_new))]
    source = ["schoolinformation.com" for i in range(len(schools_new))]


    schools_new['name'] = names
    schools_new['stabbr'] = stabbr
    schools_new['address'] = address
    schools_new['city'] = city
    schools_new['county'] = county
    schools_new['zipcode'] = zipcode
    schools_new['zip4'] = zip4
    schools_new['nces_id'] = nces_id
    schools_new['total'] = total
    schools_new['prek'] = prek
    schools_new['kinder'] = kinder
    schools_new['gr01_gr12'] = gr01_gr12
    schools_new['ungraded'] = ungraded
    schools_new['source'] = source

    schools_new['name'] = schools_new['name'].astype(str)
    schools_new['name'] = [ '\"' + i + '\"' for i in schools_new['name'] ]
    schools_new['stabbr'] = schools_new['stabbr'].astype(str)
    schools_new['stabbr'] = [ '\"' + i + '\"' for i in schools_new['stabbr'] ]
    schools_new['address'] = schools_new['address'].astype(str)
    schools_new['address'] = [ '\"' + i + '\"' for i in schools_new['address'] ]
    schools_new['city'] = schools_new['city'].astype(str)
    schools_new['city'] = [ '\"' + i + '\"' for i in schools_new['city'] ]
    schools_new['county'] = schools_new['county'].astype(str)
    schools_new['county'] = [ '\"' + i + '\"' for i in schools_new['county'] ]
    schools_new['nces_id'] = schools_new['nces_id'].astype(str)
    schools_new['nces_id'] = [ '\"' + i + '\"' for i in schools_new['nces_id'] ]

    schools_new = schools_new[['sp_id', 'name', 'stabbr', 'address', 'city', 'county', 'zipcode', 'zip4', 'nces_id', 'total', 'prek', 'kinder', 'gr01_gr12', 'ungraded', 'latitude', 'longitude', 'source', 'stco']]
    schools_new.to_csv(f'{folder}/2010_ver1_{fips}_schools.txt', sep=',', index=0, quoting=csv.QUOTE_NONE)

    ############# people.txt #############
    # gq_people_old = pd.read_csv('2010_ver1_42003_synth_gq_people.txt', sep=",") #,names=['ID','visit'])
    people_new = pd.read_csv(fips + '/' + 'people.txt', sep="\t") 
    serialno = [2007000946432 for i in range(len(people_new))]
    sporder = [1 for i in range(len(people_new))]
    sexs = list(people_new['sex'])
    sexs_done = [1 if i == 'M' else 2 for i in people_new['sex']]
    stcotrbg = np.random.choice(stcotrbgs, len(people_new))
    sp_school_id = [np.nan if i == 'X' else i for i in people_new['school_id']]
    sp_work_id = [np.nan if i == 'X' else i for i in people_new['work_id']]

    people_new['sex'] = sexs_done
    people_new['sporder'] = sporder
    people_new['serialno'] = serialno
    people_new['sporder'] = sporder
    people_new['sex'] = sexs_done
    people_new['stcotrbg'] = stcotrbg
    people_new['sp_school_id'] = sp_school_id
    people_new['sp_work_id'] = sp_work_id

    people_new['stcotrbg'] = people_new['stcotrbg'].astype(str)
    people_new['stcotrbg'] = [ '\"' + i + '\"' for i in people_new['stcotrbg'] ]
    people_new_done = people_new[['sp_id', 'sp_hh_id', 'serialno', 'stcotrbg', 'age', 'sex', 'race', 'sporder', 'relate', 'sp_school_id', 'sp_work_id']]
    people_new_done.to_csv(f'{folder}/2010_ver1_{fips}_synth_people.txt', sep=',', index=0, quoting=csv.QUOTE_NONE)

fips_list = ['42009', '42013', '42017', '42021', '42051', '42075', '42129', ]

for fips in fips_list:
    reform_data(fips)