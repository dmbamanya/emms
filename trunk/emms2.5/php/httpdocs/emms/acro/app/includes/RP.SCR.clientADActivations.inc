<?php
$date_from = $_REQUEST['date_from'] ? $_REQUEST['date_from'] : date('Y-m-d');
$date_to   = $_REQUEST['date_to']   ? $_REQUEST['date_to']   : date('Y-m-d');

$data = WEBPAGE::$dbh->getAll(sprintf("
          select
            concat(c.first,' ',c.last) client_name      ,
            c.code                     client_code      ,
            if(c.advisor_id,'A','I')   cur_status       ,
            cbz.short_name             cur_branch       ,
            cu.username                cur_advisor      ,
            u.username                 activated_by     ,
            b.short_name               activated_branch ,
            iom.date                   activated_on
         from
            (tblClients c, tblZones z, tblZones b, tblUsers u)
         left join
            tblClientIOM iom on iom.client_id = c.id       and
                                iom.type  = 'I'            and
                                iom.date != c.creator_date and
                                iom.date >= '%s'           and
                                iom.date <= '%s'
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
          'activated_by'       => WEBPAGE::$gt['activated_by'],
          'activated_branch'   => WEBPAGE::$gt['activated_at'],
          'activated_on'       => WEBPAGE::$gt['date']
        );

$pack = array
        (
          'cur_status'    => 'tblClients.status.%s'
        );

$tpl->setVariable('rpt_label',          WEBPAGE::$gt['RP.SCR.clientAD']);
$tpl->setVariable('rpt_subtitle_label', WEBPAGE::$gt['RP.SCR.clientADActivations']);
$tpl->setVariable('date_range_label',   WEBPAGE::$gt['dateRange']);
$tpl->setVariable('date_range',         sprintf('%s : %s',$date_from,$date_to));

$tpl->setVariable('chart', count($data) ? WEBPAGE::printchart($data,$head,$pack) : WEBPAGE::$gt['noData']);

?>