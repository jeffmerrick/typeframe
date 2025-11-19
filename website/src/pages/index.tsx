import type { ReactNode } from "react";
import Link from "@docusaurus/Link";
import useDocusaurusContext from "@docusaurus/useDocusaurusContext";
import Layout from "@theme/Layout";
import Heading from "@theme/Heading";
import ThemedImage from "@theme/ThemedImage";
import Image from "@theme/IdealImage";

import styles from "./index.module.css";
import useBaseUrl from "@docusaurus/useBaseUrl";

function FrameCard() {
  return (
    <div>
      <div className={styles.frameImage}>
        <div className={styles.frameImage__large}>
          <Image
            img={require("./img/typeframe-px-88-hero.png")}
            alt="Typeframe PX-88"
          />
        </div>
        <div className={styles.frameImage__simple}>
          <Image
            img={require("./img/typeframe-px-88-hero-simple.png")}
            alt="Typeframe PX-88"
          />
        </div>
      </div>

      <p className={styles.frameOverline}>
        The Typeframe PX-88 Portable Computing&nbsp;System
      </p>
      <Heading as="h1" className={styles.frameTitle}>
        A stacked deck.
      </Heading>
      <div className="row">
        <div className="col col--4 col--offset-2">
          <p>
            <strong>It's true. The odds are finally in your favor.</strong>{" "}
            <br />
            The Typeframe PX-88 is an integrated system that has been perfectly
            arranged to guarantee a superior outcome for the operator. Leave it
            to Typeframe to integrate these critical elements into one
            commanding machine.
          </p>
          <p>
            The PX-88 delivers all the power and specialized features expected
            from a professional system - but built around a dedicated,
            uncompromising user experience. Is it a cyberdeck or a writerdeck?
            It's whatever you need it to be. The reliable Raspberry Pi 4 B core
            handles demanding web-based editors and complex tasks with robust
            performance. The compact size belies the strength within.
          </p>
        </div>
        <div className="col col--4">
          <p>
            A mechanical keyboard provides a superior, tactile input experience
            - a professional tool unmatched by common consumer electronics.
            Furthermore, the system is designed for simple construction with
            minimal required soldering, and maintenance is streamlined - all
            internal components are easily reached via sliding access panels.
          </p>
          <p>
            If you have been looking for a portable, professional computer where
            input quality meets core performance, look at the PX-88.
          </p>
          <p>
            <strong>Typeframe. Built for your best work, built by you.</strong>
          </p>
          <ThemedImage
            alt="Docusaurus themed image"
            sources={{
              light: useBaseUrl("/img/logo-light.svg"),
              dark: useBaseUrl("/img/logo-dark.svg"),
            }}
            className={styles.frameLogo}
          />
        </div>
      </div>
      <div className={styles.frameButtons}>
        <Link className="button button--secondary button--lg" to="/docs/px-88/">
          Build Your PX-88
        </Link>
      </div>
    </div>
  );
}

export default function Home(): ReactNode {
  const { siteConfig } = useDocusaurusContext();
  return (
    <Layout
      title={siteConfig.title}
      description="A collection of open-source hardware and software for building writerdecks/cyberdecks."
    >
      <div className="container">
        <FrameCard />
      </div>
    </Layout>
  );
}
