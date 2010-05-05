<?php
$date_from = $_REQUEST['date_from'] ? $_REQUEST['date_from'] : date('Y-m-d');
$date_to   = $_REQUEST['date_to']   ? $_REQUEST['date_to']   : date('Y-m-d');

$data = WEBPAGE::$dbh->getAll(sprintf("
          select
            iom.advisor_id,
            concat(u.first,' ',u.last) advisor,
            c.gender,
            count(iom.id) hits,
            iom.cause,
            iom.type
          from
            tblClientIOM iom,
            tblUsers u,
            tblClients c
          where
            c.id         = iom.client_id  and
            u.id         = iom.advisor_id and
            iom.internal = '0'            and
            iom.date    >= '%s'           and
            iom.date    <= '%s'
          group by
            iom.advisor_id,
            gender,
            cause,
            type", $date_from, $date_to));

foreach($data as $key=>$val)
{
  $ldata[$val['advisor_id']]['advisor'] = str_replace(' ','&nbsp;',$val['advisor']);
  
  $ldata[$val['advisor_id']]['IN_F']   += (($val['type'] == 'I')&&($val['gender'] == 'F')) ? $val['hits'] : 0;
  $ldata[$val['advisor_id']]['IN_M']   += (($val['type'] == 'I')&&($val['gender'] == 'M')) ? $val['hits'] : 0;
  
  $ldata[$val['advisor_id']]['OUT_F']  += (($val['type'] == 'O')&&($val['gender'] == 'F')) ? $val['hits'] : 0;
  $ldata[$val['advisor_id']]['OUT_M']  += (($val['type'] == 'O')&&($val['gender'] == 'M')) ? $val['hits'] : 0;
    
  $ldata[$val['advisor_id']]['C_A']    += (($val['type'] == 'O')&&($val['cause'] == 'A'))  ? $val['hits'] : 0;
  $ldata[$val['advisor_id']]['C_B']    += (($val['type'] == 'O')&&($val['cause'] == 'B'))  ? $val['hits'] : 0;
  $ldata[$val['advisor_id']]['C_C']    += (($val['type'] == 'O')&&($val['cause'] == 'C'))  ? $val['hits'] : 0;
  $ldata[$val['advisor_id']]['C_D']    += (($val['type'] == 'O')&&($val['cause'] == 'D'))  ? $val['hits'] : 0;
  $ldata[$val['advisor_id']]['C_E']    += (($val['type'] == 'O')&&($val['cause'] == 'E'))  ? $val['hits'] : 0;
  $ldata[$val['advisor_id']]['C_F']    += (($val['type'] == 'O')&&($val['cause'] == 'F'))  ? $val['hits'] : 0;
  $ldata[$val['advisor_id']]['C_G']    += (($val['type'] == 'O')&&($val['cause'] == 'G'))  ? $val['hits'] : 0;
  $ldata[$val['advisor_id']]['C_H']    += (($val['type'] == 'O')&&($val['cause'] == 'H'))  ? $val['hits'] : 0;
  $ldata[$val['advisor_id']]['C_I']    += (($val['type'] == 'O')&&($val['cause'] == 'I'))  ? $val['hits'] : 0;
  $ldata[$val['advisor_id']]['C_J']    += (($val['type'] == 'O')&&($val['cause'] == 'J'))  ? $val['hits'] : 0;
    
  $total['advisor'] = WEBPAGE::$gt['total'];
  
  $total['IN_F']   += (($val['type'] == 'I')&&($val['gender'] == 'F')) ? $val['hits'] : 0;
  $total['IN_M']   += (($val['type'] == 'I')&&($val['gender'] == 'M')) ? $val['hits'] : 0;

  $total['OUT_F']  += (($val['type'] == 'O')&&($val['gender'] == 'F')) ? $val['hits'] : 0;
  $total['OUT_M']  += (($val['type'] == 'O')&&($val['gender'] == 'M')) ? $val['hits'] : 0;

  $total['C_A']    += (($val['type'] == 'O')&&($val['cause'] == 'A'))  ? $val['hits'] : 0;
  $total['C_B']    += (($val['type'] == 'O')&&($val['cause'] == 'B'))  ? $val['hits'] : 0;
  $total['C_C']    += (($val['type'] == 'O')&&($val['cause'] == 'C'))  ? $val['hits'] : 0;
  $total['C_D']    += (($val['type'] == 'O')&&($val['cause'] == 'D'))  ? $val['hits'] : 0;
  $total['C_E']    += (($val['type'] == 'O')&&($val['cause'] == 'E'))  ? $val['hits'] : 0;
  $total['C_F']    += (($val['type'] == 'O')&&($val['cause'] == 'F'))  ? $val['hits'] : 0;
  $total['C_G']    += (($val['type'] == 'O')&&($val['cause'] == 'G'))  ? $val['hits'] : 0;
  $total['C_H']    += (($val['type'] == 'O')&&($val['cause'] == 'H'))  ? $val['hits'] : 0;
  $total['C_I']    += (($val['type'] == 'O')&&($val['cause'] == 'I'))  ? $val['hits'] : 0;
  $total['C_J']    += (($val['type'] == 'O')&&($val['cause'] == 'J'))  ? $val['hits'] : 0;
}

if (count($ldata)) { $ldata['total'] = $total; }

$head = array
        (
          'advisor' => WEBPAGE::$gt['tblClients.advisor_id'],
          'IN_F'    => 'IN-F*',
          'IN_M'    => 'IN-M*',
          'OUT_F'   => 'OUT-F*',
          'OUT_M'   => 'OUT-M*',
          'C_A'     => 'C-A**',
          'C_B'     => 'C-B**',
          'C_C'     => 'C-C**',
          'C_D'     => 'C-D**',
          'C_E'     => 'C-E**',
          'C_F'     => 'C-F**',
          'C_G'     => 'C-G**',
          'C_H'     => 'C-H**',
          'C_I'     => 'C-I**',
          'C_J'     => 'C-J**'
        );

$tpl->setVariable('rpt_label',          WEBPAGE::$gt['clientIOM']);
$tpl->setVariable('rpt_subtitle_label', WEBPAGE::$gt['RP.SCR.clientIOMByAdvisor']);
$tpl->setVariable('date_range_label',   WEBPAGE::$gt['dateRange']);
$tpl->setVariable('date_range',         sprintf('%s : %s',$date_from,$date_to));

$tpl->setVariable('chart', count($ldata) ? WEBPAGE::printchart($ldata,$head) : WEBPAGE::$gt['noData']);

$tpl->setVariable('iomInPeriod', WEBPAGE::$gt['iomInPeriod']);
$tpl->setVariable('outCauses',   WEBPAGE::$gt['outCauses']);

$tpl->setVariable('legend',  WEBPAGE::$gt['legend']);
$tpl->setVariable('IOM_IN',  WEBPAGE::$gt['IOM_IN']);
$tpl->setVariable('IOM_OUT', WEBPAGE::$gt['IOM_OUT']);
$tpl->setVariable('F',       WEBPAGE::$gt['tblClients.gender.F']);
$tpl->setVariable('M',       WEBPAGE::$gt['tblClients.gender.M']);
$tpl->setVariable('C_A',     WEBPAGE::$gt['tblClientIOM.cause.A']);
$tpl->setVariable('C_B',     WEBPAGE::$gt['tblClientIOM.cause.B']);
$tpl->setVariable('C_C',     WEBPAGE::$gt['tblClientIOM.cause.C']);
$tpl->setVariable('C_D',     WEBPAGE::$gt['tblClientIOM.cause.D']);
$tpl->setVariable('C_E',     WEBPAGE::$gt['tblClientIOM.cause.E']);
$tpl->setVariable('C_F',     WEBPAGE::$gt['tblClientIOM.cause.F']);
$tpl->setVariable('C_G',     WEBPAGE::$gt['tblClientIOM.cause.G']);
$tpl->setVariable('C_H',     WEBPAGE::$gt['tblClientIOM.cause.H']);
$tpl->setVariable('C_I',     WEBPAGE::$gt['tblClientIOM.cause.I']);
$tpl->setVariable('C_J',     WEBPAGE::$gt['tblClientIOM.cause.J']);
?>