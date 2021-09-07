<?php

use Drupal\Core\Render\BubbleableMetadata;
use Drupal\image\Entity\ImageStyle;

/**
 * Implements hook_token_info().
 */
function malt_token_info() {
  $info = array();

  $info['tokens']['node']['title_lowercase'] = [
    'name' => t("Title (lowercase)"),
    'description' => t("The node title converted to lowercase."),
  ];
  $info['tokens']['term']['term_hashpath'] = array(
    'name' => t('Term hash path'),
    'description' => t("Path to the term built for the hash fragment"),
    'type' => 'term',
  );
  $info['tokens']['term']['term_deeppath'] = array(
    'name' => t('Term deep path'),
    'description' => t("Path to the term built only to level 2"),
    'type' => 'term',
  );
  // Return them.
  return $info;
}

/**
 * Implements hook_tokens().
 */
function malt_tokens($type, $tokens, array $data, array $options, BubbleableMetadata $bubbleable_metadata) {
  $replacements = array();
  if ($type == 'node') {
    foreach ($tokens as $name => $original) {
      // Find the desired token by name
      switch ($name) {
        case 'title_lowercase':
          $node = $data['node'];
          $replacements[$original] = strtolower($node->label());
          break;
      }
    }
  }
  if ($type == 'term' && !empty($data['term'])) {
    /* @var \Drupal\taxonomy\TermInterface $term */
    $term = $data['term'];
    /** @var \Drupal\taxonomy\TermStorageInterface $term_storage */
    $term_storage = \Drupal::entityTypeManager()->getStorage('taxonomy_term')->loadTree('categories');
    $term_data = array();
    foreach ($term_storage as $t) {
        $term_data[$t->tid] = array(
         "id" => $t->tid,
         "name" => $t->name,
         "depth"=> $t->depth
        );
    }
    foreach ($tokens as $name => $original) {
        // Find the desired token by name
        switch ($name) {
          case 'term_hashpath':
            if ($parents = token_taxonomy_term_load_all_parents($term->id(), NULL)) {
                if($term_data[$term->id()]['depth'] < 2){
                  $parents[$term->id()] = $term->getName();
                }
                $replacements[$original] = strtolower(str_replace(' ', '-', implode('-',$parents)));
            }
          break;
          case 'term_deeppath':
          if ($parents = token_taxonomy_term_load_all_parents($term->id(), NULL)) {
              if($term_data[$term->id()]['depth'] < 2){
                $parents[$term->id()] = $term->getName();
              }
              $replacements[$original] = strtolower(str_replace(' ', '-', implode('/',$parents)));
          }
          break;
        }
    }
  }
  // Return the replacements.
  return $replacements;
}
