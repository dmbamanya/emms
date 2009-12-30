<?php
function strleft($s1, $s2)
{
  return substr($s1, 0, strpos($s1, $s2));
}

$s = empty($_SERVER["HTTPS"]) ? '' : ($_SERVER["HTTPS"] == "on") ? "s" : "";
$protocol = strleft(strtolower($_SERVER["SERVER_PROTOCOL"]), "/").$s;
$port = ($_SERVER["SERVER_PORT"] == "80") ? "" : (":".$_SERVER["SERVER_PORT"]);
$url = $protocol."://".$_SERVER['SERVER_NAME'].$port.$_SERVER['REQUEST_URI'];

$path = array_reverse(explode('/',parse_url($url,PHP_URL_PATH)));
array_pop($path);
array_shift($path);

foreach($path as $i => $val)
{
  $location = sprintf('%s/%s',$val,$location);
}
$location = $protocol."://".$_SERVER['SERVER_NAME'].'/'.$location.'app';

header(sprintf("Location: %s",$location));
exit;
?>