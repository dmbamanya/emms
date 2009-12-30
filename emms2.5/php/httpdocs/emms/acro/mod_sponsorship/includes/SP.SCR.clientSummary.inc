<?php

$show_gohome = true;

$data = WEBPAGE::$dbh->getAll(sprintf("
          select
            z.zone,
            count(c.id) qty,
            c.gender,
            round(AVG(YEAR(NOW())-YEAR(c.birthdate))) age,
            round(avg(c.dependants),1) dep
          from
            tblClients c,
            tblLoans l,
            tblZones z,
            tblLoansMasterDetails lmd,
            tblLoansMaster lm
          where
            c.id               = l.client_id   and
            z.id               = l.zone_id     and
            lmd.loan_id        = l.id          and
            lm.id              = lmd.master_id and
            lm.sponsor_id      = %s            and
            YEAR(c.birthdate) < YEAR(NOW())-10 and
            YEAR(NOW())-YEAR(c.birthdate) < 90
          group by
            l.zone_id,c.gender",WEBPAGE::$userID)); 

foreach($data as $key=>$val)
{
  $mrow[$val['zone']]['zone']  = '';
  $mrow[$val['zone']]['qty_F'] = '';
  $mrow[$val['zone']]['qty_M'] = '';
  $mrow[$val['zone']]['age_F'] = '';
  $mrow[$val['zone']]['age_M'] = '';
  $mrow[$val['zone']]['dep_F'] = '';
  $mrow[$val['zone']]['dep_M'] = '';
}

foreach($data as $key=>$val)
{
  $mrow[$val['zone']]['zone']                           = $val['zone'];
  $mrow[$val['zone']][sprintf('qty_%s',$val['gender'])] = $val['qty'];
  $mrow[$val['zone']][sprintf('age_%s',$val['gender'])] = $val['age'];
  $mrow[$val['zone']][sprintf('dep_%s',$val['gender'])] = $val['dep'];
}

$extra['zone']['header']   = 'tblZones.zone';
$extra['zone']['pack']     = '%s';
$extra['zone']['align']    = 'left';

$extra['qty_F']['header']  = 'qty_F';
$extra['qty_F']['pack']    = '%s';
$extra['qty_F']['align']   = 'right';

$extra['qty_M']['header']  = 'qty_M';
$extra['qty_M']['pack']    = '%s';
$extra['qty_M']['align']   = 'right';

$extra['age_F']['header']  = 'age_F';
$extra['age_F']['pack']	   = '%s';
$extra['age_F']['align']   = 'right';

$extra['age_M']['header']  = 'age_M';
$extra['age_M']['pack']	   = '%s';
$extra['age_M']['align']   = 'right';

$extra['dep_F']['header']  = 'dep_F';
$extra['dep_F']['pack']	   = '%s';
$extra['dep_F']['align']   = 'right';

$extra['dep_M']['header']  = 'dep_M';
$extra['dep_M']['pack']	   = '%s';
$extra['dep_M']['align']   = 'right';

$extra['param']['ref']     = WEBPAGE::$scr_name;
$extra['param']['ts']      = microtime(true);

array_unshift($mrow,$extra);

WEBPAGE::makecachefile($mrow,WEBPAGE::$queryCache.'.'.$extra['param']['ts']);
require_once './includes/BS.SCR.pager.inc';

?>

