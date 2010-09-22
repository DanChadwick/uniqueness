<?php
// $Id$

/**
 * @file
 * Settings page for the uniqueness module.
 */

/**
 * Form builder for uniqueness settings page; system_settings_form().
 */
function uniqueness_settings() {
  $form = array();

  // appearance
  $form['appearance'] = array(
    '#type' => 'fieldset',
    '#title' => t('Appearance'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['appearance']['uniqueness_default_title'] = array(
    '#type' => 'textfield',
    '#title' => t('Default title'),
    '#default_value' => variable_get('uniqueness_default_title', t('Related content')),
    '#description' => t('Note: when using the widget as a block then this title can be overriden by the block title on the !block_settings_page.',
        array('!block_settings_page' => l(t('uniqueness block settings page'), 'admin/build/block/configure/uniqueness/uniqueness'))),
    '#required' => TRUE,
  );
  $form['appearance']['uniqueness_default_description'] = array(
    '#type' => 'textarea',
    '#title' => t('Default description'),
    '#default_value' => variable_get('uniqueness_default_description',
        t("Help us increase the signal to noise ratio! If we find content that's related or similar to what you're posting it will be listed here.")),
    '#rows' => 2,
  );

  return system_settings_form($form);
}
