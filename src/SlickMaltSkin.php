<?php

namespace Drupal\malt;

use Drupal\slick\SlickSkinInterface;
use Drupal\Core\Url;

/**
 * Implements SlickSkinInterface as registered via hook_slick_skins_info().
 */

class SlickMaltSkin implements SlickSkinInterface {

  /**
   * {@inheritdoc}
   */
  public function skins() {
    $base = Url::fromUri('internal:/')->setAbsolute()->toString();
    $module_handler = \Drupal::service('module_handler');
    $path = $module_handler->getModule('malt')->getPath();
    $skins = [
      'x_testimonial' => [
        'name' => 'Rental items carousel',
        'description' => t('Carousel for displaying rental items'),
        'group' => 'main',
        'provider' => 'malt',
        'css' => [
          'theme' => [
            $base . $path . '/css/slick.theme--rental-items.css' => [],
          ],
        ],
      ],
    ];

    return $skins;
  }

}
