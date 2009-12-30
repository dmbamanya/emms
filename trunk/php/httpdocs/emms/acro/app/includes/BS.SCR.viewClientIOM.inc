<?
require_once 'class/roles.php';
require_once 'class/client.php';

if ($_GET['id'])
{
  $client = new CLIENT($_GET[id]);

  $extra['param']['value']['id']              = $_GET['id'];
  $extra['param']['label']['tblClients.id']   = $client->data[name];
  $extra['param']['label']['tblClients.code'] = $client->data[code];
  $extra['param']['popup']                    = true;

  $fields[]                                   = sprintf("DATE_FORMAT(iom.date,'%s') AS date",WEBPAGE::$gt['date_format_mysql']);
  $extra['date']['header']                    = 'tblClientIOM.date';
  $extra['date']['pack']                      = '%s';
  $extra['date']['align']                     = 'left';
    
  $fields[]                                   = "iom.type";
  $extra['type']['header']                    = 'tblClientIOM.type';
  $extra['type']['pack']                      = "%s";
  $extra['type']['align']                     = 'center';

  $fields[]                                   = "iom.cause";
  $extra['cause']['header']                   = 'tblClientIOM.cause';
  $extra['cause']['pack']                     = '%s';
  $extra['cause']['align']                    = 'center';
  
  $fields[]                                   = "ua.username AS advisor";
  $extra['advisor']['header']                 = 'tblClientIOM.advisor_id';
  $extra['advisor']['pack']                   = '%s';
  $extra['advisor']['align']                  = 'center';

  $fields[]                                   = "uc.username AS creator";
  $extra['creator']['header']                 = 'tblClientIOM.user_id';
  $extra['creator']['pack']                   = '%s';
  $extra['creator']['align']                  = 'center';
  
  $fields[]                                   = "CONCAT((LPAD(z.id,3,'0')),'.',s.category,'.',(LPAD(s.id,5,'0'))) AS society";
  $extra['society']['header']                 = 'tblClientIOM.society_id';
  $extra['society']['pack']                   = '%s';
  $extra['society']['align']                  = 'center';
  
  $fields[]                                   = "p.program AS program";
  $extra['program']['header']                 = 'tblPrograms.program';
  $extra['program']['pack']                   = '%s';
  $extra['program']['align']                  = 'center';

  $extra['param']['ref']                      = WEBPAGE::$scr_name;
  $extra['param']['ts']                       = microtime();

  $param = sprintf("
             iom.client_id = '%s'           and
             c.id          = iom.client_id  and
             ua.id         = iom.advisor_id and
             uc.id         = iom.user_id    and
             z.id          = iom.zone_id    and
             p.id          = z.program_id", $_GET['id']);
  
  $tables = 'tblClientIOM iom';
  $left   = 'tblSocieties s';
  $on     = 's.id = iom.society_id, tblClients c, tblUsers ua, tblUsers uc, tblZones z, tblPrograms p';

  $mrow = SQL::select_leftjoin($tables, $fields, $left, $on, $param);
  foreach ($mrow as $key=>$val)
  {
    $mrow[$key]['type']  = WEBPAGE::$gt[sprintf("tblClientIOM.type.%s",$val['type'])];
    $mrow[$key]['cause'] = WEBPAGE::$gt[sprintf("tblClientIOM.cause.%s",$val['cause'])];
  }
  array_unshift($mrow,$extra);
  WEBPAGE::makecachefile($mrow,WEBPAGE::$queryCache.'.'.$extra['param']['ts']);
  require_once 'BS.SCR.pager.inc';
}    

?>