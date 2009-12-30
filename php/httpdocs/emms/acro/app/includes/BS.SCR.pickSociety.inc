<?
require_once 'class/society.php';

if (($_GET['zone_id'])&&($_GET['advisor_id']))
{
  $soc = new SOCIETY($_GET['parent_id']);

  $extra['param']['label']['tblSocieties.zone_id']    = $_GET['zone_id'];
  $extra['param']['label']['tblSocieties.advisor_id'] = $_GET['advisor_id'];

  $fields[]                = "s.name";
  $extra['name']['header'] = 'tblSocieties.name';
  $extra['name']['pack']   = '%s';
  $extra['name']['align']  = 'left';
    
  $fields[]                = "concat((lpad(z.id,3,'0')),'.',s.category,'.',(lpad(s.id,5,'0'))) code";
  $extra['code']['header'] = 'tblSocieties.code';
  $extra['code']['pack']   = '%s';
  $extra['code']['align']  = 'center';

  $fields[]                = "concat('\"',s.id,'\",\"',s.name,'\"') pick";
  $extra['pick']['header'] = 'pickSociety';
  $extra['pick']['pack']   = sprintf("<a href='javascript:updateSociety(%s)'>%s</a>",'%s',WEBPAGE::$gt['pickSociety']);
  $extra['pick']['align']  = 'center';

  $extra['param']['ref']   = WEBPAGE::$scr_name;
  $extra['param']['ts']    = microtime(true);

  $param = sprintf("
             s.category != 'BG' and
             s.zone_id = '%s' and
             s.advisor_id = '%s' and
             z.id = s.zone_id", $_GET['zone_id'],
                                $_GET['advisor_id']);
  
  $mrow = SQL::select('tblSocieties s, tblZones z', $fields, $param);
  array_unshift($mrow,$extra);

  WEBPAGE::makecachefile($mrow,WEBPAGE::$queryCache.'.'.$extra['param']['ts']);
  require_once 'BS.SCR.pager.inc';
}
    
?>