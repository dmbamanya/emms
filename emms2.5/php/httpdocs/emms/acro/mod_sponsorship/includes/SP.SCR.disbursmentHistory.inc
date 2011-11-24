<?php

$show_gohome = true;

$extra['kp']['header']       = 'tblLoans.id';
$extra['kp']['pack']         = '%s';
$extra['kp']['align']        = 'right';

$extra['donation']['header'] = 'donation';
$extra['donation']['pack']   = '%s';
$extra['donation']['align']  = 'right';

$extra['date']['header']     = 'date';
$extra['date']['pack']       = '%s';
$extra['date']['align']      = 'right';

$extra['client']['header']   = 'tblClients.id';
$extra['client']['pack']     = '%s';
$extra['client']['align']    = 'right';

$extra['zone']['header']     = 'tblZones.zone';
$extra['zone']['pack']       = '%s';
$extra['zone']['align']      = 'right';

$extra['program']['header']  = 'tblPrograms.program';
$extra['program']['pack']    = '%s';
$extra['program']['align']   = 'right';

$extra['officer']['header']  = 'tblClients.advisor_id';
$extra['officer']['pack']    = '%s';
$extra['officer']['align']   = 'right';

$extra['view']['header']     = 'details';
$extra['view']['pack']       = sprintf("<a href='javascript:openWin(\"index.php?popup=1&scr_name=SP.SCR.viewLoan&ref=SP.SCR.disbursmentHistory&id=%s\",\"%s\",\"menubar=no,scrollbars=yes,resizable=yes,width=720,height=600\")' onClick=''>%s</a>", '%s',mktime(),WEBPAGE::$gt['view']);
$extra['view']['align']      = 'center';

$extra['param']['ref']       = WEBPAGE::$scr_name;
$extra['param']['ts']        = microtime(true);

$mrow = WEBPAGE::$dbh->getAll(sprintf("
  select
    lm.amount kp,
    sum(lms.donation) donation,
    date_format(max(lms.datetime),'%%Y-%%m-%%d') date,
    coalesce(lg.group_name, concat(c.first, ' ', substr(c.last,1,1))) client,
    z.zone,
    p.program,
    concat(u.first, ' ', substr(u.last,1,1)) officer,
    lms.master_id view
  from
    tblLoansMasterSponsors lms,
    tblLoansMaster lm,
    tblPrograms p,
    tblZones z,
    tblUsers u,
    tblClients c,
    view_loan_group lg,
    (
      select
        lmd.master_id,
        min(lmd.loan_id) id,
        l.advisor_id,
        l.client_id
      from
        tblLoansMasterDetails lmd,
        tblLoans l
      where
        l.id = lmd.loan_id
      group by
        lmd.master_id
    ) l
  where
    lms.sponsor_id = %s and
    lm.id = lms.master_id and
    p.id = lm.program_id and
    z.id = lm.zone_id and
    l.master_id = lms.master_id and
    u.id = l.advisor_id and
    c.id = l.client_id and
    lg.id = l.id
  group by
    lms.master_id", WEBPAGE::$userID));

array_unshift($mrow,$extra);

WEBPAGE::makecachefile($mrow,WEBPAGE::$queryCache.'.'.$extra['param']['ts']);
require_once './includes/BS.SCR.pager.inc';

?>