<?php
$date_from = $_REQUEST['date_from'] ? $_REQUEST['date_from'] : date('Y-m-d');
$date_to   = $_REQUEST['date_to']   ? $_REQUEST['date_to']   : date('Y-m-d');

$data = WEBPAGE::$dbh->getAll(sprintf("
          select
            concat(c.first,' ',c.last) client_name        ,
            c.code                     client_code        ,
            if(c.advisor_id,'A','I')   cur_status         ,
            cbz.short_name             cur_branch         ,
            cu.username                cur_advisor        ,
            u.username                 deactivated_by     ,
            b.short_name               deactivated_branch ,
            iom.date                   deactivated_on     ,
            iom.cause                  deactivated_cause
          from
            (tblClients c, tblZones z, tblZones b, tblUsers u)
          left join
            tblClientIOM iom on iom.client_id  = c.id and
                                iom.type       = 'O'  and
                                iom.date      >= '%s' and
                                iom.date      <= '%s'
          left join
            tblZones     cz  on cz.id  = c.zone_id
          left join
            tblZones     cbz on cbz.id = cz.parent_id
          left join
            tblUsers     cu  on cu.id  = c.advisor_id
          where
            z.id = iom.zone_id and
            b.id = z.parent_id and
            u.id = iom.user_id", $date_from, $date_to));

$head = array
        (
          'client_name'        => WEBPAGE::$gt['tblClients.id'],
          'client_code'        => WEBPAGE::$gt['tblClients.code'],
          'cur_status'         => WEBPAGE::$gt['tblClients.status'],
          'cur_branch'         => WEBPAGE::$gt['tblZones.zone'],
          'cur_advisor'        => WEBPAGE::$gt['tblClients.advisor_id'],
          'deactivated_by'     => WEBPAGE::$gt['deactivated_by'],
          'deactivated_branch' => WEBPAGE::$gt['deactivated_at'],
          'deactivated_on'     => WEBPAGE::$gt['date'],
          'deactivated_cause'  => WEBPAGE::$gt['tblClientIOM.cause']
        );

$pack = array
        (
          'cur_status'        => 'tblClients.status.%s',
          'deactivated_cause' => 'tblClientIOM.cause.%s'
        );

$tpl->setVariable('rpt_label',          WEBPAGE::$gt['RP.SCR.clientAD']);
$tpl->setVariable('rpt_subtitle_label', WEBPAGE::$gt['RP.SCR.clientADDeactivations']);
$tpl->setVariable('date_range_label',   WEBPAGE::$gt['dateRange']);
$tpl->setVariable('date_range',         sprintf('%s : %s',$date_from,$date_to));

$tpl->setVariable('chart', count($data) ? WEBPAGE::printchart($data,$head,$pack) : WEBPAGE::$gt['noData']);

?>
