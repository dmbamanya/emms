#!/usr/bin/php
<?php
$path = '/base_path_to_conf.ini_file/emms/acro';
$conf = parse_ini_file($path . '/conf.ini', true);

$linkID = @mysql_connect($conf['db']['host'],$conf['db']['usr'],$conf['db']['pwd']) or die("Could not connect to MySQL server");
@mysql_select_db($conf['db']['name']) or die("Cold not select db");
$r = mysql_query("show tables like 'tbl%'");
mysql_close();
$tbl = '';
while($row = mysql_fetch_array($r))
{
  $tbl = $tbl . " " . $row[0];
}

$cmd[] = sprintf("mysqldump -u %s --password=%s --opt %s %s | gzip > %s/backup/prebk.gz",$conf['db']['usr'],$conf['db']['pwd'],$conf['db']['name'],$tbl,$path);
$cmd[] = sprintf("lynx -dump %sindex.cron.php",$conf['app']['url']);
$cmd[] = sprintf("mysqldump -u %s --password=%s --opt %s %s | gzip > %s/backup/postbk.gz",$conf['db']['usr'],$conf['db']['pwd'],$conf['db']['name'],$tbl,$path);
$cmd[] = sprintf("lynx -dump %sindex.mail.risk.php",$conf['app']['url']);

foreach($cmd as $key => $val)
{
  shell_exec($val);
}

?>