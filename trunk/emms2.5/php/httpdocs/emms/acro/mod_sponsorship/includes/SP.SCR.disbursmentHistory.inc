<?php

$show_gohome = true;

$extra['kp']['header']      = 'tblLoans.kp';
$extra['kp']['pack']        = '%s';
$extra['kp']['align']       = 'right';

$extra['date']['header']    = 'date';
$extra['date']['pack']      = '%s';
$extra['date']['align']     = 'right';

$extra['client']['header']  = 'tblClients.id';
$extra['client']['pack']    = '%s';
$extra['client']['align']   = 'right';

$extra['zone']['header']    = 'tblZones.zone';
$extra['zone']['pack']      = '%s';
$extra['zone']['align']     = 'right';

$extra['program']['header'] = 'tblPrograms.program';
$extra['program']['pack']   = '%s';
$extra['program']['align']  = 'right';

$extra['view']['header']    = 'details';
$extra['view']['pack']      = sprintf("<a href='javascript:openWin(\"index.php?popup=1&scr_name=SP.SCR.viewLoan&ref=SP.SCR.disbursmentHistory&id=%s\",\"%s\",\"menubar=no,scrollbars=yes,resizable=yes,width=720,height=600\")' onClick=''>%s</a>", '%s',mktime(),WEBPAGE::$gt['view']);
$extra['view']['align']     = 'center';

$extra['param']['ref']      = WEBPAGE::$scr_name;
$extra['param']['ts']       = microtime(true);

$mrow = WEBPAGE::$dbh->getAll(sprintf("
          select
            l.kp,
            lsh.date,
            concat(c.first,' ',c.last) client,
            z.zone,
            p.program,
            l.id view
          from
            tblLoans l,
            tblLoanStatusHistory lsh,
            tblLoansMasterDetails lmd,
            tblLoansMaster lm,
            tblClients c,
            tblZones z,
            tblPrograms p
          where
            lmd.loan_id   = l.id and
            lm.id         = lmd.master_id and
            lm.sponsor_id = '%s'          and
            lsh.loan_id   = l.id          and
            lsh.status    = 'D'           and
            c.id          = l.client_id   and
            p.id          = l.program_id  and
            z.id          = l.zone_id
          order by
            lsh.date desc",WEBPAGE::$userID));

array_unshift($mrow,$extra);

WEBPAGE::makecachefile($mrow,WEBPAGE::$queryCache.'.'.$extra['param']['ts']);
require_once './includes/BS.SCR.pager.inc';

?>